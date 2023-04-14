#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtQml/QQmlListProperty>

#include "dbadapter.h"

class WeatherData : public QObject {
    Q_OBJECT
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
    void setData(QJsonObject forecast);

    bool isValid();

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

    QSqlDatabase m_db;
};
#endif // WEATHERDATA_H
