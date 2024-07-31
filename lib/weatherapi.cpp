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
    connect(m_api, &OpenWeatherAPI::searchCityDataReady, this, &WeatherAPI::formatListFromNameSearch);
    connect(m_api, &OpenWeatherAPI::findCitybyGeo, this, &WeatherAPI::geoCityReady);
    connect(m_api, &OpenWeatherAPI::cityNotFound, this, &WeatherAPI::cityNotFound);
    connect(m_api, &OpenWeatherAPI::weatherChanged, this, &WeatherAPI::weatherChanged);
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
