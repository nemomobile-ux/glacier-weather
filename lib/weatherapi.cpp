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

#include "weatherapi.h"
#include "openweatherapi.h"
#include "dbadapter.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlError>

WeatherAPI::WeatherAPI(QObject *parent)
    : QObject{parent}
    , m_api(new OpenWeatherAPI(this))
{
    connect(m_api.get(), &OpenWeatherAPI::searchCityDataReady, this, &WeatherAPI::formatListFromNameSearch);
    connect(m_api.get(), &OpenWeatherAPI::findCitybyGeo, this, &WeatherAPI::geoCityReady);
    connect(m_api.get(), &OpenWeatherAPI::cityNotFound, this, &WeatherAPI::cityNotFound);
    connect(m_api.get(), &OpenWeatherAPI::weatherChanged, this, &WeatherAPI::weatherChanged);
}

bool WeatherAPI::addToFavorites(int cityID, QString cityName)
{
    QSqlDatabase db = dbAdapter::instance().getDatabase();

    QSqlQuery query(db);
    query.prepare("INSERT INTO cityes (`cityID`, `cityName`) VALUES (:cityID, :cityName)");
    query.bindValue(":cityID", cityID);
    query.bindValue(":cityName", cityName);

    return query.exec();
}

bool WeatherAPI::isFavoriteCity(QString cityID)
{
    QSqlDatabase db = dbAdapter::instance().getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT id FROM cityes WHERE cityID=:cityID");
    query.bindValue(":cityID", cityID);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }
    if (query.next()) {
        return true;
    }
    return false;
}

void WeatherAPI::removeCityFromFavorites(int cityID)
{
    QSqlDatabase db = dbAdapter::instance().getDatabase();

    QSqlQuery query(db);
    query.prepare("DELETE FROM cityes WHERE cityID=:cityID");
    query.bindValue(":cityID", cityID);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }
}

void WeatherAPI::searchCityByCoord(const QGeoCoordinate coord)
{
    m_api->searchCityByCoord(coord);
}

void WeatherAPI::refreshWeather(QString city)
{
    m_api->refreshWeather(city);
}

QList<WeatherAPI::Place> WeatherAPI::formatPlacesList()
{
    QList<WeatherAPI::Place> placesList;
    QSqlDatabase db = dbAdapter::instance().getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM cityes");

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }

    while (query.next()) {
        WeatherAPI::Place place;
        place.cityId = query.value(2).toInt();
        place.cityName = query.value(1).toString();
        place.lat = 0;
        place.lon = 0;

        placesList.push_back(place);
    }
    return placesList;
}

QList<WeatherData> WeatherAPI::localCityWatcher(QString city)
{
    QList<WeatherData> weatcherList;
    if(city.isEmpty()) {
        return weatcherList;
    }

    QSqlDatabase db = dbAdapter::instance().getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT id FROM weather WHERE cityName=:City ORDER BY datetime DESC");
    query.bindValue(":City", city);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
        return weatcherList;
    }

    while (query.next()) {
        WeatherData weatcher(query.value(0).toInt());
        if (weatcher.isValid()) {
            weatcherList.push_back(weatcher);
        }
    }
    return weatcherList;
}

void WeatherAPI::formatListFromNameSearch(QByteArray json)
{
    QList<Place> places;
    QJsonDocument document = QJsonDocument::fromJson(json);

    for (int i = 0; i < document.array().count(); i++) {
        WeatherAPI::Place place;
        place.cityId = 0;
        place.cityName = document.array().at(i).toObject().value("display_name").toString();
        place.lat = document.array().at(i).toObject().value("lat").toString().toDouble();
        place.lon = document.array().at(i).toObject().value("lon").toString().toDouble();

        qDebug() << place.cityId << place.cityName << place.lat << place.lon;

        places.push_back(place);
    }

    emit searchCityDataReady(places);
}
