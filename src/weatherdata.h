#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtQml/QQmlListProperty>

class WeatherData : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString dayOfWeek
            READ dayOfWeek WRITE setDayOfWeek
                NOTIFY dataChanged)
    Q_PROPERTY(QString weatherIcon
            READ weatherIcon WRITE setWeatherIcon
                NOTIFY dataChanged)
    Q_PROPERTY(QString weatherDescription
            READ weatherDescription WRITE setWeatherDescription
                NOTIFY dataChanged)
    Q_PROPERTY(QString temperature
            READ temperature WRITE setTemperature
                NOTIFY dataChanged)
    Q_PROPERTY(QString windSpeed
            READ windSpeed WRITE setWindSpeed
                NOTIFY dataChanged)
    Q_PROPERTY(QString windGusts
            READ windGusts WRITE setWindGusts
                NOTIFY dataChanged)
    Q_PROPERTY(QString windDirection
            READ windDirection WRITE setWindDirection
                NOTIFY dataChanged)

public:
    explicit WeatherData(QObject* parent = 0);
    WeatherData(const WeatherData& other);

    QString dayOfWeek() const;
    QString weatherIcon() const;
    QString weatherDescription() const;
    QString temperature() const;
    QString windSpeed() const;
    QString windGusts() const;
    QString windDirection() const;

    void setDayOfWeek(const QString& value);
    void setWeatherIcon(const QString& value);
    void setWeatherDescription(const QString& value);
    void setTemperature(const QString& value);
    void setWindSpeed(const QString& value);
    void setWindGusts(const QString& value);
    void setWindDirection(const QString& value);

signals:
    void dataChanged();

private:
    QString m_dayOfWeek;
    QString m_weather;
    QString m_weatherDescription;
    QString m_temperature;
    QString m_wind_speed;
    QString m_wind_gusts;
    QString m_wind_direction;
};

Q_DECLARE_METATYPE(WeatherData)

#endif // WEATHERDATA_H
