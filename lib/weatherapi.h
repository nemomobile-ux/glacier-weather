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

#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QGeoCoordinate>
#include "weatherdata.h"

class OpenWeatherAPI;
class WeatherAPI : public QObject
{
    Q_OBJECT
public:
    struct Place {
        int dbID;
        int cityId;
        QString cityName;
        double lat;
        double lon;
    };

    explicit WeatherAPI(QObject *parent = nullptr);
    bool addToFavorites(int cityID, QString cityName);
    bool isFavoriteCity(QString cityID);
    void removeCityFromFavorites(int cityID);
    void searchCityByCoord(const QGeoCoordinate coord);
    void refreshWeather(QString city);
    void refreshWeather(QGeoCoordinate coord);

    QList<Place> formatPlacesList();
    QList<WeatherData> localCityWatcher(QString city);

signals:
    void searchCityDataReady(QList<Place> places);
    void geoCityReady(QString city);
    void cityNotFound(QString city);
    void weatherChanged();

private slots:
    void formatListFromNameSearch(QByteArray json);

private:
    std::shared_ptr<OpenWeatherAPI> m_api;

};

#endif // WEATHERAPI_H
