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

Item {
    id: current

    property var temperatureMin
    property var temperatureMax

    property int windDirection
    property var windSpeed

    property string bottomText: "Mostly cloudy"
    property string weatherIcon: "01d"
    property real smallSide: (current.width < current.height ? current.width : current.height)
    property bool pressed: false;

    function formatTemp() {
        return weatherModel.niceTemperatureString(settings.temperatureUnits , temperatureMin)
                + " / "
                + weatherModel.niceTemperatureString(settings.temperatureUnits, temperatureMax)
                + settings.temperatureUnitsOptions.get(settings.temperatureUnits).shortname
    }

    Column {
        id: textColumn
        spacing: Theme.itemSpacingSmall;
        anchors {
            top: current.top
            left: current.left
            topMargin: Theme.itemSpacingMedium
            leftMargin: Theme.itemSpacingMedium
        }
        width: parent.width/3

        Text {
            text: formatTemp()
            color: pressed ? Theme.accentColor : Theme.textColor
            font.pixelSize: Theme.fontSizeExtraLarge * 3
        }
        Text {
            text: current.windDirection + "° "
            font.pixelSize: Theme.fontSizeExtraLarge
            color: pressed ? Theme.accentColor : Theme.textColor

        }
        Text {
            text: Number(weatherModel.speedConvert(settings.windUnits, current.windSpeed)).toFixed(2) + " " + settings.speedUnitsOptions.get(settings.windUnits).shortname
            color: pressed ? Theme.accentColor : Theme.textColor
            font.pixelSize: Theme.fontSizeExtraLarge
        }
        Text {
            color: pressed ? Theme.accentColor : Theme.textColor
            font.pixelSize: Theme.fontSizeLarge
            wrapMode: Text.WordWrap
            width: parent.width
        }
    }

    WeatherIcon {
        weatherIcon: current.weatherIcon
        anchors.top: parent.top;
        anchors.left: textColumn.right
        anchors.right: parent.right;
        anchors.margins: Theme.itemSpacingLarge
        width: smallSide
        height: smallSide
        pressed: current.pressed
    }

}
