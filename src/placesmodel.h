#ifndef PLACESMODEL_H
#define PLACESMODEL_H

#include <qgeopositioninfo.h>
#include <qgeopositioninfosource.h>
#include <qgeosatelliteinfosource.h>
#include <qnmeapositioninfosource.h>

#include <QElapsedTimer>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

#include "openweatherapi.h"

class PlacesModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged)
    Q_PROPERTY(bool useLocation READ useLocation NOTIFY useLocationChanged)

    struct Place {
        int dbID;
        int cityId;
        QString cityName;
        double lat;
        double lon;
    };

public:
    explicit PlacesModel(QObject* parent = nullptr);
    ~PlacesModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_hash; }

    Q_INVOKABLE void searchByLocation();
    Q_INVOKABLE void addToFavorites(int cityID, QString cityName);
    Q_INVOKABLE void removeFromFavorites(int cityID);

    QString searchString() { return m_searchSctring; }
    void setSearchString(QString searchString);

    Q_INVOKABLE void getCityData(int index);
    Q_INVOKABLE bool isFavorite(QString cityID);

    Q_INVOKABLE QVariantMap get(int row) const;

    bool useLocation() { return m_useLocation; }

signals:
    void searchStringChanged(QString string);
    void geoCityReady(QString city);
    void useLocationChanged();

public slots:
    void positionUpdated(QGeoPositionInfo gpsPos);
    void positionError(QGeoPositionInfoSource::Error e);

private slots:
    void formatListFromNameSearch(QByteArray json);
    void formatListFromDB();

private:
    void loadPlaces(QString string);

    QSqlDatabase m_db;
    OpenWeatherAPI* m_api;
    QGeoPositionInfoSource* src;

    QHash<int, QByteArray> m_hash;
    QList<Place> m_placesList;

    QString m_searchSctring;
    bool m_useLocation;
};

#endif // PLACESMODEL_H
