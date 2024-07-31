#ifndef OPENWEATHERAPI_H
#define OPENWEATHERAPI_H

#include "weatherdata.h"
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
