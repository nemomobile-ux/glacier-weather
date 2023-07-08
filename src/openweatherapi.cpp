#include "openweatherapi.h"
#include "weatherdata.h"

#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QStringList>
#include <QTimer>
#include <QUrlQuery>

/*
 *This application uses http://openweathermap.org/api
 **/

OpenWeatherAPI::OpenWeatherAPI(QObject* parent)
    : QObject(parent)
    , m_temperatureUnits(0)
    , m_windUnits(0)
{
    m_requestNewWeatherTimer.setSingleShot(false);
    m_requestNewWeatherTimer.setInterval(20 * 60 * 1000); // 20 min
    m_requestNewWeatherTimer.start();

    m_nam = new QNetworkAccessManager(this);
}

OpenWeatherAPI::~OpenWeatherAPI()
{
}

void OpenWeatherAPI::refreshWeather(const QString& city)
{
    if (city.isEmpty()) {
        qDebug() << "refreshing weather skipped (no city)";
        return;
    }
    qDebug() << "refreshing weather";

    QUrl url("http://api.openweathermap.org/data/2.5/forecast/daily");
    QUrlQuery query;

    query.addQueryItem("q", city);
    query.addQueryItem("mode", "json");
    query.addQueryItem("cnt", "5");
    query.addQueryItem("APPID", m_appIdent);
    url.setQuery(query);

    QNetworkReply* rep = m_nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished,
        this, [this, rep, city]() { handleForecastNetworkData(rep, city); });
}

void OpenWeatherAPI::handleForecastNetworkData(QNetworkReply* networkReply, QString forecastCityName)
{
    qDebug() << "got forecast";
    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        QJsonValue jv;
        QJsonObject root = document.object();

        QJsonObject jaCity = root.value("city").toObject();
        int forecastCityId = jaCity.value("id").toInt();

        if (forecastCityId == 0 || forecastCityName.isEmpty()) {
            qWarning() << "unknow city";
            return;
        }

        jv = root.value(QStringLiteral("list"));
        if (!jv.isArray()) {
            qWarning() << "Invalid forecast object";
            return;
        }

        QJsonArray ja = jv.toArray();
        // we need 4 days of forecast -> first entry is today
        if (ja.count() != 5) {
            qWarning() << "Invalid forecast object";
            return;
        }

        for (int i = 0; i < ja.count(); i++) {
            WeatherData* forecastEntry = new WeatherData(0);

            QJsonObject subtree = ja.at(i).toObject();
            subtree.insert("cityID", forecastCityId);
            subtree.insert("cityName", forecastCityName);

            forecastEntry->setData(subtree);
        }

        emit weatherChanged();
    } else {
        emit networkDataError(forecastCityName, networkReply->errorString());

        if (networkReply->errorString().contains("Not Found")) {
            emit cityNotFound(forecastCityName);
        }
    }

    networkReply->deleteLater();
}

void OpenWeatherAPI::handleGeoNetworkData(QNetworkReply* networkReply)
{
    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());
        const QString city = document.array().at(0).toObject().value("name").toString();
        qDebug() << "got city: " << city;

        emit findCitybyGeo(city);
    }

    networkReply->deleteLater();
}

int OpenWeatherAPI::temperatureUnits()
{
    return m_temperatureUnits;
}

void OpenWeatherAPI::setTemperatureUnits(int& value)
{
    m_temperatureUnits = value;
    emit temperatureUnitsChanged();
}

int OpenWeatherAPI::windUnits()
{
    return m_windUnits;
}

void OpenWeatherAPI::setWindUnits(int& value)
{
    m_windUnits = value;
    emit windUnitsChanged();
}

void OpenWeatherAPI::searchCityByCoord(QGeoCoordinate coord)
{
    qDebug() << "requested query of city";

    QString latitude, longitude;
    longitude.setNum(coord.longitude());
    latitude.setNum(coord.latitude());

    QUrl url("http://api.openweathermap.org/geo/1.0/reverse");
    QUrlQuery query;
    query.addQueryItem("lat", latitude);
    query.addQueryItem("lon", longitude);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", m_appIdent);
    url.setQuery(query);
    qDebug() << "submitting request";

    QNetworkReply* rep = m_nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished,
        this, [this, rep]() { handleGeoNetworkData(rep); });
}
