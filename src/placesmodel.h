/****************************************************************************************
**
** Copyright (C) 2025 Chupligin Sergey <neochapay@gmail.com>
** All rights reserved.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the author nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

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
#include <QAbstractItemModel>

#include <weatherapi.h>

class PlacesModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged)
    Q_PROPERTY(bool useLocation READ useLocation NOTIFY useLocationChanged)

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
    void formatListFromNameSearch(QList<WeatherAPI::Place> places);
    void formatListFromDB();

private:
    void loadPlaces(QString string);

    std::shared_ptr<WeatherAPI> m_weatherAPI;
    QGeoPositionInfoSource* src;

    QHash<int, QByteArray> m_hash;
    QList<WeatherAPI::Place> m_placesList;

    QString m_searchSctring;
    bool m_useLocation;
};

#endif // PLACESMODEL_H
