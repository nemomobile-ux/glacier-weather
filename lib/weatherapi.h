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
    OpenWeatherAPI* m_api;

};

#endif // WEATHERAPI_H
