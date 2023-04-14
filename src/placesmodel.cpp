#include <QtNetwork/QNetworkReply>
#include <qgeopositioninfo.h>
#include <qgeopositioninfosource.h>
#include <qgeosatelliteinfosource.h>
#include <qnetworkconfigmanager.h>
#include <qnmeapositioninfosource.h>

#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QStringList>
#include <QTimer>
#include <QUrlQuery>

#include <QDebug>

#include "placesmodel.h"

PlacesModel::PlacesModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_useLocation(false)
{
    m_hash.insert(Qt::UserRole, QByteArray("dbID"));
    m_hash.insert(Qt::UserRole + 1, QByteArray("cityID"));
    m_hash.insert(Qt::UserRole + 2, QByteArray("cityName"));
    m_hash.insert(Qt::UserRole + 3, QByteArray("lat"));
    m_hash.insert(Qt::UserRole + 4, QByteArray("lon"));

    m_api = new OpenWeatherAPI();
    m_db = dbAdapter::instance().getDatabase();

    src = QGeoPositionInfoSource::createDefaultSource(this);
    connect(src, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
    connect(src, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(positionError(QGeoPositionInfoSource::Error)));

    connect(this, &PlacesModel::searchStringChanged, this, &PlacesModel::loadPlaces);
    connect(m_api, &OpenWeatherAPI::searchCityDataReady, this, &PlacesModel::formatListFromNameSearch);
    connect(m_api, &OpenWeatherAPI::findCitybyGeo, this, &PlacesModel::geoCityReady);

    formatListFromDB();
}

PlacesModel::~PlacesModel()
{
    if (src) {
        src->stopUpdates();
    }
}

int PlacesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_placesList.count();
}

QVariant PlacesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_placesList.size())
        return QVariant();

    Place item = m_placesList.at(index.row());
    switch (role) {
    case Qt::UserRole:
        return item.dbID;
    case Qt::UserRole + 1:
        return item.cityId;
    case Qt::UserRole + 2:
        return item.cityName;
    case Qt::UserRole + 3:
        return item.lat;
    case Qt::UserRole + 4:
        return item.lon;
    default:
        return QVariant();
    }
}

void PlacesModel::searchByLocation()
{
    if (src) {
        src->startUpdates();
    }
}

void PlacesModel::addToFavorites(int cityID, QString cityName)
{
    if (cityID == 0 || cityName.isEmpty()) {
        return;
    }

    qDebug() << Q_FUNC_INFO << cityID << cityName;
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO cityes (`cityID`, `cityName`) VALUES (:cityID, :cityName)");
    query.bindValue(":cityID", cityID);
    query.bindValue(":cityName", cityName);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }
}

bool PlacesModel::isFavorite(QString cityID)
{
    qDebug() << cityID;
    if (cityID.isEmpty()) {
        return false;
    }

    QSqlQuery query(m_db);
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

QVariantMap PlacesModel::get(int row) const
{
    QVariantMap res;
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QModelIndex idx = index(row, 0);
    while (i.hasNext()) {
        i.next();
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }

    return res;
}

void PlacesModel::removeFromFavorites(int cityID)
{
    if (cityID == 0) {
        return;
    }
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM cityes WHERE cityID=:cityID");
    query.bindValue(":cityID", cityID);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }
}

void PlacesModel::setSearchString(QString searchString)
{
    if (searchString != m_searchSctring) {
        m_searchSctring = searchString;
        emit searchStringChanged(searchString);
    }
}

void PlacesModel::getCityData(int index)
{
    m_useLocation = false;
    emit useLocationChanged();

    Place item = m_placesList.at(index);
    QGeoCoordinate coord;
    coord.setLatitude(item.lat);
    coord.setLongitude(item.lon);

    m_api->searchCityByCoord(coord);
}

void PlacesModel::positionUpdated(QGeoPositionInfo gpsPos)
{
    m_useLocation = true;
    emit useLocationChanged();

    src->stopUpdates();
    m_api->searchCityByCoord(gpsPos.coordinate());
}

void PlacesModel::positionError(QGeoPositionInfoSource::Error e)
{
    Q_UNUSED(e);
    qWarning() << "Position source error. Falling back to simulation mode.";
    // cleanup insufficient QGeoPositionInfoSource instance
    src->stopUpdates();
    src->deleteLater();
}

void PlacesModel::formatListFromNameSearch(QByteArray json)
{
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    m_placesList.clear();
    QJsonDocument document = QJsonDocument::fromJson(json);

    for (int i = 0; i < document.array().count(); i++) {
        Place place;
        place.cityId = 0;
        place.cityName = document.array().at(i).toObject().value("display_name").toString();
        place.lat = document.array().at(i).toObject().value("lat").toString().toDouble();
        place.lon = document.array().at(i).toObject().value("lon").toString().toDouble();

        qDebug() << place.cityId << place.cityName << place.lat << place.lon;

        m_placesList.push_back(place);
    }
    endResetModel();
}

void PlacesModel::formatListFromDB()
{
    beginResetModel();
    m_placesList.clear();

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM cityes");

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }

    while (query.next()) {
        Place place;
        place.cityId = query.value(2).toInt();
        place.cityName = query.value(1).toString();
        place.lat = 0;
        place.lon = 0;

        m_placesList.push_back(place);
    }

    endResetModel();
}

void PlacesModel::loadPlaces(QString string)
{
    if (string.isEmpty()) {
        // load from DB
        formatListFromDB();
    } else {
        // search data
    }
}
