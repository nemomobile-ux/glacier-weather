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
    : QObject(parent)
{
    src = QGeoPositionInfoSource::createDefaultSource(this);
    bool useGps = true;
    connect(src, SIGNAL(positionUpdated(QGeoPositionInfo)),
        this, SLOT(positionUpdated(QGeoPositionInfo)));
    connect(src, SIGNAL(error(QGeoPositionInfoSource::Error)),
        this, SLOT(positionError(QGeoPositionInfoSource::Error)));

    delayedCityRequestTimer.setSingleShot(true);
    delayedCityRequestTimer.setInterval(1000); // 1 s
    throttle.invalidate();

    app_ident = QStringLiteral("36496bad1955bf3365448965a42b9eac");

    nam = new QNetworkAccessManager(this);

    QObject::connect(&delayedCityRequestTimer, SIGNAL(timeout()), this, SLOT(queryCity()));

    // do not use gps if not needed
    //    src->startUpdates();
    m_city = "";
}

PlacesModel::~PlacesModel()
{
    if (src) {
        src->stopUpdates();
    }
}

void PlacesModel::update()
{
    src->startUpdates();

    m_city = "";
    throttle.invalidate();
    emit cityChanged();
}

QString PlacesModel::city()
{
    return m_city;
}

void PlacesModel::positionUpdated(QGeoPositionInfo gpsPos)
{
    coord = gpsPos.coordinate();
    src->stopUpdates();
    queryCity();
}

void PlacesModel::queryCity()
{
    // don't update more often then once a minute
    // to keep load on server low
    if (throttle.isValid() && throttle.elapsed() < minMsBeforeNewRequest) {
        qDebug() << "delaying query of city";
        if (!delayedCityRequestTimer.isActive())
            delayedCityRequestTimer.start();
        return;
    }
    qDebug() << "requested query of city";
    throttle.start();
    minMsBeforeNewRequest = (nErrors + 1) * baseMsBeforeNewRequest;

    QString latitude, longitude;
    longitude.setNum(coord.longitude());
    latitude.setNum(coord.latitude());

    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("lat", latitude);
    query.addQueryItem("lon", longitude);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", app_ident);
    url.setQuery(query);
    qDebug() << "submitting request";

    QNetworkReply* rep = nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished,
        this, [this, rep]() { handleGeoNetworkData(rep); });
}

void PlacesModel::positionError(QGeoPositionInfoSource::Error e)
{
    Q_UNUSED(e);
    qWarning() << "Position source error. Falling back to simulation mode.";
    // cleanup insufficient QGeoPositionInfoSource instance
    src->stopUpdates();
    src->deleteLater();
    src = 0;

    // activate simulation mode
    m_city = "Brisbane";
    emit cityChanged();
}

void PlacesModel::hadError(bool tryAgain)
{
    qDebug() << "hadError, will " << (tryAgain ? "" : "not ") << "rety";
    throttle.start();
    if (nErrors < 10)
        ++nErrors;
    minMsBeforeNewRequest = (nErrors + 1) * baseMsBeforeNewRequest;
    if (tryAgain)
        delayedCityRequestTimer.start();
}

void PlacesModel::handleGeoNetworkData(QNetworkReply* networkReply)
{
    if (!networkReply) {
        hadError(false); // should retry?
        return;
    }

    if (!networkReply->error()) {
        nErrors = 0;
        if (!throttle.isValid())
            throttle.start();
        minMsBeforeNewRequest = baseMsBeforeNewRequest;
        // convert coordinates to city name
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        QJsonObject jo = document.object();
        QJsonValue jv = jo.value(QStringLiteral("name"));

        const QString city = jv.toString();
        qDebug() << "got city: " << city;
        if (m_city != city) {
            m_city = city;
            emit cityChanged();
        }
    } else {
        hadError(true);
    }
    networkReply->deleteLater();
}
