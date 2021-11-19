/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "appmodel.h"
#include "weatherdata.h"

#include <qnetworkconfigmanager.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QTimer>
#include <QUrlQuery>
#include <QElapsedTimer>
#include <QLoggingCategory>

/*
 *This application uses http://openweathermap.org/api
 **/

#define ZERO_KELVIN 273.15

static QString niceTemperatureString(double t)
{
    return QString::number(qRound(t-ZERO_KELVIN)) + QChar(0xB0);
}


class AppModelPrivate
{
public:
    static const int baseMsBeforeNewRequest = 5 * 1000; // 5 s, increased after each missing answer up to 10x
    QNetworkAccessManager *nam;

    WeatherData now;
    QList<WeatherData*> forecast;
    QQmlListProperty<WeatherData> *fcProp;
    bool ready;
    int nErrors;
    int minMsBeforeNewRequest;
    QTimer requestNewWeatherTimer;

    QString app_ident;
    QString city;

    AppModelPrivate() :
        nam(NULL),
        fcProp(NULL),
        ready(false),
        nErrors(0),
        minMsBeforeNewRequest(baseMsBeforeNewRequest)
    {
        requestNewWeatherTimer.setSingleShot(false);
        requestNewWeatherTimer.setInterval(20*60*1000); // 20 min
        app_ident = QStringLiteral("36496bad1955bf3365448965a42b9eac");
    }
};

static void forecastAppend(QQmlListProperty<WeatherData> *prop, WeatherData *val)
{
    Q_UNUSED(val);
    Q_UNUSED(prop);
}

static WeatherData *forecastAt(QQmlListProperty<WeatherData> *prop, int index)
{
    AppModelPrivate *d = static_cast<AppModelPrivate*>(prop->data);
    return d->forecast.at(index);
}

static int forecastCount(QQmlListProperty<WeatherData> *prop)
{
    AppModelPrivate *d = static_cast<AppModelPrivate*>(prop->data);
    return d->forecast.size();
}

static void forecastClear(QQmlListProperty<WeatherData> *prop)
{
    static_cast<AppModelPrivate*>(prop->data)->forecast.clear();
}



AppModel::AppModel(QObject *parent) :
        QObject(parent),
        d(new AppModelPrivate)
{
    d->fcProp = new QQmlListProperty<WeatherData>(this, d,
                                                          forecastAppend,
                                                          forecastCount,
                                                          forecastAt,
                                                          forecastClear);

    connect(&d->requestNewWeatherTimer, SIGNAL(timeout()), this, SLOT(refreshWeather()));
    d->requestNewWeatherTimer.start();

    d->nam = new QNetworkAccessManager(this);
    d->city = "";
    this->refreshWeather();
}

AppModel::~AppModel()
{
    delete d;
}



void AppModel::refreshWeather()
{
    if (d->city.isEmpty()) {
        qDebug() << "refreshing weather skipped (no city)";
        return;
    }
    qDebug() << "refreshing weather";
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;

    query.addQueryItem("q", d->city);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", d->app_ident);
    url.setQuery(query);

    QNetworkReply *rep = d->nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished,
            this, [this, rep]() { handleWeatherNetworkData(rep); });
}


void AppModel::handleWeatherNetworkData(QNetworkReply *networkReply)
{
    qDebug() << "got weather network data";
    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
        foreach (WeatherData *inf, d->forecast)
            delete inf;
        d->forecast.clear();

        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        if (document.isObject()) {
            QJsonObject obj = document.object();
            QJsonObject tempObject;
            QJsonValue val;

            if (obj.contains(QStringLiteral("weather"))) {
                val = obj.value(QStringLiteral("weather"));
                QJsonArray weatherArray = val.toArray();
                val = weatherArray.at(0);
                tempObject = val.toObject();
                d->now.setWeatherDescription(tempObject.value(QStringLiteral("description")).toString());
                d->now.setWeatherIcon(tempObject.value("icon").toString());

            }
            if (obj.contains(QStringLiteral("wind"))) {
                val = obj.value(QStringLiteral("wind"));
                tempObject = val.toObject();
                val = tempObject.value(QStringLiteral("speed"));
                d->now.setWindSpeed(QString::number(qRound(val.toDouble())));
                val = tempObject.value(QStringLiteral("gust"));
                d->now.setWindGusts(QString::number(qRound(val.toDouble())));
                val = tempObject.value(QStringLiteral("deg"));
                d->now.setWindDirection(QString::number(qRound(val.toDouble())));
            }
            if (obj.contains(QStringLiteral("main"))) {
                val = obj.value(QStringLiteral("main"));
                tempObject = val.toObject();
                val = tempObject.value(QStringLiteral("temp"));
                d->now.setTemperature(niceTemperatureString(val.toDouble()));
            }
        }
    }
    networkReply->deleteLater();

    //retrieve the forecast
    QUrl url("http://api.openweathermap.org/data/2.5/forecast/daily");
    QUrlQuery query;

    query.addQueryItem("q", d->city);
    query.addQueryItem("mode", "json");
    query.addQueryItem("cnt", "5");
    query.addQueryItem("APPID", d->app_ident);
    url.setQuery(query);

    QNetworkReply *rep = d->nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished,
            this, [this, rep]() { handleForecastNetworkData(rep); });
}

void AppModel::handleForecastNetworkData(QNetworkReply *networkReply)
{
    qDebug() << "got forecast";
    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        QJsonObject jo;
        QJsonValue jv;
        QJsonObject root = document.object();
        jv = root.value(QStringLiteral("list"));
        if (!jv.isArray())
            qWarning() << "Invalid forecast object";
        QJsonArray ja = jv.toArray();
        //we need 4 days of forecast -> first entry is today
        if (ja.count() != 5)
            qWarning() << "Invalid forecast object";

        QString data;
        for (int i = 1; i<ja.count(); i++) {
            WeatherData *forecastEntry = new WeatherData();

            //min/max temperature
            QJsonObject subtree = ja.at(i).toObject();

            jo = subtree.value(QStringLiteral("temp")).toObject();
            jv = jo.value(QStringLiteral("min"));
            data.clear();
            data += niceTemperatureString(jv.toDouble());
            data += QChar('/');
            jv = jo.value(QStringLiteral("max"));
            data += niceTemperatureString(jv.toDouble());
            forecastEntry->setTemperature(data);

            jv = subtree.value(QStringLiteral("speed"));
            forecastEntry->setWindSpeed(QString::number(jv.toDouble()));

            jv = subtree.value(QStringLiteral("gust"));
            forecastEntry->setWindGusts(QString::number(jv.toDouble()));

            jv = subtree.value(QStringLiteral("deg"));
            forecastEntry->setWindDirection(QString::number(jv.toDouble()));

            //get date
            jv = subtree.value(QStringLiteral("dt"));
            QDateTime dt = QDateTime::fromMSecsSinceEpoch((qint64)jv.toDouble()*1000);
            forecastEntry->setDayOfWeek(dt.date().toString(QStringLiteral("ddd")));

            //get icon
            QJsonArray weatherArray = subtree.value(QStringLiteral("weather")).toArray();
            jo = weatherArray.at(0).toObject();
            forecastEntry->setWeatherIcon(jo.value(QStringLiteral("icon")).toString());

            //get description
            forecastEntry->setWeatherDescription(jo.value(QStringLiteral("description")).toString());

            d->forecast.append(forecastEntry);
        }

        emit weatherChanged();
    }
    networkReply->deleteLater();
}

bool AppModel::hasValidCity() const
{
    return (!(d->city.isEmpty()) && d->city.size() > 1 && d->city != "");
}

bool AppModel::hasValidWeather() const
{
    return hasValidCity() && (!(d->now.weatherIcon().isEmpty()) &&
                              (d->now.weatherIcon().size() > 1) &&
                              d->now.weatherIcon() != "");
}

WeatherData *AppModel::weather() const
{
    return &(d->now);
}

QQmlListProperty<WeatherData> AppModel::forecast() const
{
    return *(d->fcProp);
}


QString AppModel::city() const
{
    return d->city;
}

void AppModel::setCity(const QString &value)
{
    d->city = value;
    emit cityChanged();
    refreshWeather();
}
