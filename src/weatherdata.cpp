#include "weatherdata.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QSqlQuery>

WeatherData::WeatherData(int id)
    : m_id(id)
{
    m_db = dbAdapter::instance().getDatabase();

    if (id != 0) {
        // LOAD DATA FROM DB
        getWeatherByID(m_id);
    }
    connect(this, &WeatherData::dataChanged, this, &WeatherData::update);
}

void WeatherData::setData(QJsonObject forecast)
{
    bool needToUpdate = false;

    QString cityName = forecast.value("cityName").toString();
    int cityID = forecast.value("cityID").toInt();
    double dateTime = forecast.value("dt").toDouble();
    QString weatherIcon = forecast.value("weather").toArray().at(0).toObject().value("icon").toString();
    QString weatherDescription = forecast.value("weather").toArray().at(0).toObject().value("description").toString();
    double temperatureMin = forecast.value("temp").toObject().value("min").toDouble();
    double temperatureMax = forecast.value("temp").toObject().value("min").toDouble();
    double windSpeed = forecast.value("speed").toDouble();
    double windGusts = forecast.value("gusts").toDouble();
    int windDirection = forecast.value("deg").toInt();

    if (m_id == 0) {
        getWeatherByDateTime(dateTime);
    }

    if (m_cityName != cityName || m_cityID != cityID || m_dateTime != dateTime || m_weatherIcon != weatherIcon || m_weatherDescription != weatherDescription || m_temperatureMin != temperatureMin || m_temperatureMax != temperatureMax || m_windSpeed != windSpeed || m_windGusts != windGusts || m_windDirection != windDirection) {
        needToUpdate = true;
    }

    m_cityName = cityName;
    m_cityID = cityID;
    m_dateTime = dateTime;
    m_weatherIcon = weatherIcon;
    m_weatherDescription = weatherDescription;
    m_temperatureMin = temperatureMin;
    m_temperatureMax = temperatureMax;
    m_windSpeed = windSpeed;
    m_windGusts = windGusts;
    m_windDirection = windDirection;

    if (m_id == 0) {
        insert();
        return;
    }

    if (needToUpdate) {
        emit dataChanged();
    }
}

bool WeatherData::isValid()
{
    return (m_cityID != 0);
}

QString WeatherData::cityName() const
{
    return m_cityName;
}

int WeatherData::cityID() const
{
    return m_cityID;
}

int WeatherData::dateTime() const
{
    return m_dateTime;
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
    return m_weatherIcon;
}

QString WeatherData::weatherDescription() const
{
    return m_weatherDescription;
}

double WeatherData::temperatureMin() const
{
    return m_temperatureMin;
}

double WeatherData::temperatureMax() const
{
    return m_temperatureMax;
}

double WeatherData::windSpeed() const
{
    return m_windSpeed;
}

double WeatherData::windGusts() const
{
    return m_windGusts;
}

int WeatherData::windDirection() const
{
    return m_windDirection;
}

void WeatherData::setCityName(const QString& value)
{
    if (value != m_cityName) {
        m_cityName = value;
        emit dataChanged();
    }
}

void WeatherData::setCityID(const int& value)
{
    if (value != m_cityID) {
        m_cityID = value;
        emit dataChanged();
    }
}

void WeatherData::setDateTime(const double& value)
{
    if (value != m_dateTime) {
        m_dateTime = value;
        emit dataChanged();
    }
}

void WeatherData::setWeatherIcon(const QString& value)
{
    if (value != m_weatherIcon) {
        m_weatherIcon = value;
        emit dataChanged();
    }
}

void WeatherData::setWeatherDescription(const QString& value)
{
    if (m_weatherDescription != value) {
        m_weatherDescription = value;
        emit dataChanged();
    }
}

void WeatherData::setTemperatureMin(const double& value)
{
    if (m_temperatureMin != value) {
        m_temperatureMin = value;
        emit dataChanged();
    }
}

void WeatherData::setTemperatureMax(const double& value)
{
    if (m_temperatureMax != value) {
        m_temperatureMax = value;
        emit dataChanged();
    }
}

void WeatherData::setWindSpeed(const double& value)
{
    if (m_windSpeed != value) {
        m_windSpeed = value;
        emit dataChanged();
    }
}

void WeatherData::setWindGusts(const double& value)
{
    if (m_windGusts != value) {
        m_windGusts = value;
        emit dataChanged();
    }
}

void WeatherData::setWindDirection(const int& value)
{
    if (m_windDirection != value) {
        m_windDirection = value;
        emit dataChanged();
    }
}

void WeatherData::getWeatherByDateTime(int dateTime)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM weather WHERE dateTime=:dateTime");
    query.bindValue(":dateTime", dateTime);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }

    if (query.next()) {
        m_id = query.value(0).toInt();
        m_cityName = query.value(1).toString();
        m_cityID = query.value(2).toInt();
        m_dateTime = query.value(3).toInt();
        m_weatherIcon = query.value(4).toString();
        m_weatherDescription = query.value(5).toString();
        m_temperatureMin = query.value(6).toDouble();
        m_temperatureMax = query.value(7).toDouble();
        m_windSpeed = query.value(8).toDouble();
        m_windGusts = query.value(9).toDouble();
        m_windDirection = query.value(10).toInt();
    }
}

void WeatherData::getWeatherByID(int id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM weather WHERE id=:id");
    query.bindValue(":id", id);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }

    if (query.next()) {
        m_cityName = query.value(1).toString();
        m_cityID = query.value(2).toInt();
        m_dateTime = query.value(3).toInt();
        m_weatherIcon = query.value(4).toString();
        m_weatherDescription = query.value(5).toString();
        m_temperatureMin = query.value(6).toDouble();
        m_temperatureMax = query.value(7).toDouble();
        m_windSpeed = query.value(8).toDouble();
        m_windGusts = query.value(9).toDouble();
        m_windDirection = query.value(10).toInt();
    }
}

void WeatherData::insert()
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO weather (`cityName`,`cityID`,`dateTime`,`weatherIcon`,`weatherDescription`,`temperatureMin`,`temperatureMax`,`windSpeed`,`windGusts`,`windDirection`)"
                  "VALUES (:cityName, :cityID, :dateTime, :weatherIcon, :weatherDescription, :temperatureMin, :temperatureMax, :windSpeed, :windGusts, :windDirection)");
    query.bindValue(":cityName", m_cityName);
    query.bindValue(":cityID", m_cityID);
    query.bindValue(":dateTime", m_dateTime);
    query.bindValue(":weatherIcon", m_weatherIcon);
    query.bindValue(":weatherDescription", m_weatherDescription);
    query.bindValue(":temperatureMin", m_temperatureMin);
    query.bindValue(":temperatureMax", m_temperatureMax);
    query.bindValue(":windSpeed", m_windSpeed);
    query.bindValue(":windGusts", m_windGusts);
    query.bindValue(":windDirection", m_windDirection);

    bool ok = query.exec();
    if (!ok) {
        qWarning() << query.lastQuery() << query.lastError().text();
    }
    m_id = query.lastInsertId().toInt();
    emit dataChanged();
}

void WeatherData::update()
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE weather SET cityName=:CityName"
                  ", cityID=:CityID"
                  ", dateTime=:DateTime"
                  ", weatherIcon=:WeatherIcon"
                  ", weatherDescription=:WeatherDescription"
                  ", temperatureMin=:TemperatureMin"
                  ", temperatureMax=:TemperatureMax"
                  ", windSpeed=:WindSpeed"
                  ", windGusts=:WindGusts"
                  ", windDirection=:WindDirection"
                  " WHERE id=:wid");
    query.bindValue(":wid", m_id);
    query.bindValue(":CityName", m_cityName);
    query.bindValue(":CityID", m_cityID);
    query.bindValue(":DateTime", m_dateTime);
    query.bindValue(":WeatherIcon", m_weatherIcon);
    query.bindValue(":WeatherDescription", m_weatherDescription);
    query.bindValue(":TemperatureMin", m_temperatureMin);
    query.bindValue(":TemperatureMax", m_temperatureMax);
    query.bindValue(":WindSpeed", m_windSpeed);
    query.bindValue(":WindGusts", m_windGusts);
    query.bindValue(":WindDirection", m_windDirection);

    bool ok = query.exec();
    if (!ok) {
        qWarning() << query.lastQuery() << query.lastError().text();
    }
}
