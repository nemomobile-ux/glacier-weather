#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include "weatherapi.h"
#include "weatherdata.h"
#include <QAbstractListModel>

#define ZERO_KELVIN 273.15

class WeatherModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(bool hasValidWeather READ hasValidWeather NOTIFY hasValidWeatherChanged)

public:
    explicit WeatherModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_hash; }

    QString city() { return m_city; }
    void setCity(QString city);

    Q_INVOKABLE QVariantMap get(int row) const;
    bool hasValidWeather();

    Q_INVOKABLE QString niceTemperatureString(int temperatureUnits, double t);
    Q_INVOKABLE double speedConvert(int units, double s);

    Q_INVOKABLE void loadWeatherFromAPI();

signals:
    void cityChanged();
    void hasValidWeatherChanged();
    void cityNotFound(QString city);

private:
    QHash<int, QByteArray> m_hash;
    QString m_city;
    WeatherAPI* m_weatherAPI;
    QList<WeatherData> m_weatcherList;

    void loadWeatherFromDB();
};

#endif // WEATHERMODEL_H
