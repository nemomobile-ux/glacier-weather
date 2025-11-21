/****************************************************************************************
**
** Copyright (C) 2025 Chupligin Sergey <neochapay@gmail.com>
** All rights reserved.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the author nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

#ifndef OPENWEATHERAPI_H
#define OPENWEATHERAPI_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtQml/QQmlListProperty>

#include <QtPositioning/QGeoPositionInfoSource>

#include <QTimer>

#define ZERO_KELVIN 273.15

class OpenWeatherAPI : public QObject {
    Q_OBJECT
    Q_PROPERTY(int temperatureUnits READ temperatureUnits WRITE setTemperatureUnits NOTIFY temperatureUnitsChanged)
    Q_PROPERTY(int windUnits READ windUnits WRITE setWindUnits NOTIFY windUnitsChanged)

public:
    explicit OpenWeatherAPI(QObject* parent = 0);
    ~OpenWeatherAPI();

    int temperatureUnits();
    void setTemperatureUnits(int& value);

    int windUnits();
    void setWindUnits(int& value);

    void searchCityByCoord(const QGeoCoordinate coord);

public slots:
    void refreshWeather(const QString& city);

private slots:
    void handleForecastNetworkData(QNetworkReply* networkReply, QString forecastCityName);
    void handleGeoNetworkData(QNetworkReply* networkReply);

signals:
    void weatherChanged();
    void cityChanged();
    void temperatureUnitsChanged();
    void windUnitsChanged();

    void findCitybyGeo(QString city);
    void searchCityDataReady(QByteArray json);

    void networkDataError(QString city, QString errorString);
    void cityNotFound(QString city);

private:
    QString m_appIdent = "36496bad1955bf3365448965a42b9eac";
    QString m_currentCity;
    QTimer m_requestNewWeatherTimer;
    QNetworkAccessManager* m_nam;

    int m_nErrors;
    int m_minMsBeforeNewRequest;
    int m_temperatureUnits;
    int m_windUnits;
};

#endif // OPENWEATHERAPI_H
