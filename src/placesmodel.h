#ifndef PLACESMODEL_H
#define PLACESMODEL_H

#include <qgeopositioninfosource.h>
#include <qgeosatelliteinfosource.h>
#include <qnmeapositioninfosource.h>
#include <qgeopositioninfo.h>

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class PlacesModel : public QObject
{
    Q_OBJECT

public:
    explicit PlacesModel(QObject *parent = nullptr);
    ~PlacesModel();

    Q_INVOKABLE void update();
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)


signals:
    void cityChanged();

public slots:
    QString city();
    void queryCity();
    void positionUpdated(QGeoPositionInfo gpsPos);
    void positionError(QGeoPositionInfoSource::Error e);
    void handleGeoNetworkData(QNetworkReply *networkReply);
    void hadError(bool tryAgain);


private:
    QGeoPositionInfoSource *src;
    QGeoCoordinate coord;
    QString longitude, latitude;
    QString m_city;


    QNetworkAccessManager *nam;
    QElapsedTimer throttle;
    QTimer delayedCityRequestTimer;
    int minMsBeforeNewRequest;
    QString app_ident;
    int nErrors;
    static const int baseMsBeforeNewRequest = 5 * 1000; // 5 s, increased after each missing answer up to 10x

signals:

};

#endif // PLACESMODEL_H
