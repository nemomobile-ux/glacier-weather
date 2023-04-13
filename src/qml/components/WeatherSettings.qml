/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0

import WeatherInfo 1.0

Item {

    Settings  {
        id: settingsStorage;
    }

    property int temperatureUnits: 1; // Kelvin, Celsius, Farnheit
    property int windUnits: 1 // Mph, m/s, km/h, kt
    property alias places: placesModel
    property alias speedUnitsOptions: speedUnitModel
    property alias temperatureUnitsOptions: temperatureUnitModel
    property var selectedCity
    property bool settingsReady: false

    signal placesModelChanged()

    ListModel {
        id: speedUnitModel
        ListElement { name: qsTr("Miles"); shortname: qsTr("Mph"); }
        ListElement { name: qsTr("Meters per seconds"); shortname: qsTr("m/s"); }
        ListElement { name: qsTr("Kilometers per hour"); shortname: qsTr("km/h"); }
        ListElement { name: qsTr("Knots"); shortname: qsTr("kt"); }
    }

    ListModel {
        id: temperatureUnitModel
        ListElement { name: qsTr("Kelvin"); shortname: qsTr("K"); }
        ListElement { name: qsTr("Celsius"); shortname: qsTr("°C"); }
        ListElement { name: qsTr("Fahrenheit"); shortname: qsTr("°F"); }
    }

    ListModel {
        id: placesModel

        onCountChanged: {
            if (settingsReady) {
                settingsSavePlacesModel();
                placesModelChanged()
            }
        }
    }

    onWindUnitsChanged: {
        if (settingsReady) {
            settingsStorage.setValue("windUnits", windUnits)
        }
    }

    onTemperatureUnitsChanged: {
        if (settingsReady) {
            settingsStorage.setValue("temperatureUnits", temperatureUnits)
        }
    }

    onSelectedCityChanged: {
        if (settingsReady) {
            settingsStorage.setValue("selectedCity", selectedCity)
        }
    }

    function settingsSavePlacesModel() {
        var citiesArr = []
        for (var i = 0; i < placesModel.count; i++) {
            var item = placesModel.get(i)
            if (item.useGps) {
                continue;
            }
            citiesArr.push(item.city);
        }

        var json_string = JSON.stringify(citiesArr)
        settingsStorage.setValue("cities", json_string)

    }

    function settingsLoadPlacesModel() {
        placesModel.clear();
        var json = settingsStorage.value("cities", "[]");
        var citiesArray = JSON.parse(json);
        for (var i = 0; i < citiesArray.length; i++) {
            placesModel.append({city: citiesArray[i], useGps: false})
        }
        placesModel.append({city: qsTr("Current position"), useGps: true})
    }

    function removeCity(city) {
        var json = settingsStorage.value("cities", "[]");
        var citiesArray = JSON.parse(json);
        for (var i = 0; i < citiesArray.length; i++) {
            if(citiesArray[i] != city) {
                placesModel.append({city: citiesArray[i], useGps: false})
            }
        }
        settingsSavePlacesModel()
        settingsLoadPlacesModel();
    }

    Component.onCompleted: {
        settingsLoadPlacesModel();
        windUnits     = settingsStorage.value("windUnits", 1)
        temperatureUnits =  parseInt(settingsStorage.value("temperatureUnits", 1))
        selectedCity = settingsStorage.value("selectedCity", "");
        settingsReady = true;
    }

}
