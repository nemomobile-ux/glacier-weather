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

#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtQml/QQmlListProperty>

class WeatherData : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY dataChanged)
    Q_PROPERTY(int cityID READ cityID WRITE setCityID NOTIFY dataChanged)
    Q_PROPERTY(int dateTime READ dateTime WRITE setDateTime NOTIFY dataChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY dataChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY dataChanged)
    Q_PROPERTY(double temperatureMin READ temperatureMin WRITE setTemperatureMin NOTIFY dataChanged)
    Q_PROPERTY(double temperatureMax READ temperatureMax WRITE setTemperatureMax NOTIFY dataChanged)

    Q_PROPERTY(double windSpeed READ windSpeed WRITE setWindSpeed NOTIFY dataChanged)
    Q_PROPERTY(double windGusts READ windGusts WRITE setWindGusts NOTIFY dataChanged)
    Q_PROPERTY(int windDirection READ windDirection WRITE setWindDirection NOTIFY dataChanged)

public:
    explicit WeatherData(int id);
    WeatherData(const WeatherData& other);
    WeatherData& operator=(const WeatherData& other);

    void setData(QJsonObject forecast);

    bool isValid();

    int id() const;
    QString cityName() const;
    int cityID() const;
    int dateTime() const;
    QString weatherIcon() const;
    QString weatherDescription() const;
    double temperatureMin() const;
    double temperatureMax() const;
    double windSpeed() const;
    double windGusts() const;
    int windDirection() const;

    void setCityName(const QString& value);
    void setCityID(const int& value);
    void setDateTime(const double& value);
    void setWeatherIcon(const QString& value);
    void setWeatherDescription(const QString& value);
    void setTemperatureMin(const double& value);
    void setTemperatureMax(const double& value);
    void setWindSpeed(const double& value);
    void setWindGusts(const double& value);
    void setWindDirection(const int& value);

signals:
    void dataChanged();

private:
    void getWeatherByDateTime(int dateTime);
    void getWeatherByID(int id);
    void insert();
    void update();

    int m_id;
    QString m_cityName;
    int m_cityID;
    int m_dateTime;
    QString m_weatherIcon;
    QString m_weatherDescription;
    double m_temperatureMin;
    double m_temperatureMax;
    double m_windSpeed;
    double m_windGusts;
    int m_windDirection;
};
#endif // WEATHERDATA_H
