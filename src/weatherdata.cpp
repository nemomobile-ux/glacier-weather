#include "weatherdata.h"

WeatherData::WeatherData(QObject *parent) :
        QObject(parent)
{
}

WeatherData::WeatherData(const WeatherData &other) :
        QObject(0),
        m_dayOfWeek(other.m_dayOfWeek),
        m_weather(other.m_weather),
        m_weatherDescription(other.m_weatherDescription),
        m_temperature(other.m_temperature)
{
}

QString WeatherData::dayOfWeek() const
{
    return m_dayOfWeek;
}

/*!
 * The icon value is based on OpenWeatherMap.org icon set. For details
 * see http://bugs.openweathermap.org/projects/api/wiki/Weather_Condition_Codes
 *
 * e.g. 01d ->sunny day
 *
 * The icon string will be translated to
 * http://openweathermap.org/img/w/01d.png
 */
QString WeatherData::weatherIcon() const
{
    return m_weather;
}

QString WeatherData::weatherDescription() const
{
    return m_weatherDescription;
}

QString WeatherData::temperature() const
{
    return m_temperature;
}

QString WeatherData::windSpeed() const
{
    return m_wind_speed;
}

QString WeatherData::windGusts() const
{
    return m_wind_gusts;
}

QString WeatherData::windDirection() const
{
    return m_wind_direction;
}

void WeatherData::setDayOfWeek(const QString &value)
{
    m_dayOfWeek = value;
    emit dataChanged();
}

void WeatherData::setWeatherIcon(const QString &value)
{
    m_weather = value;
    emit dataChanged();
}

void WeatherData::setWeatherDescription(const QString &value)
{
    m_weatherDescription = value;
    emit dataChanged();
}

void WeatherData::setTemperature(const QString &value)
{
    m_temperature = value;
    emit dataChanged();
}

void WeatherData::setWindSpeed(const QString &value)
{
    m_wind_speed = value;
    emit dataChanged();
}

void WeatherData::setWindGusts(const QString &value)
{
    m_wind_gusts = value;
    emit dataChanged();
}

void WeatherData::setWindDirection(const QString &value)
{
    m_wind_direction = value;
}


