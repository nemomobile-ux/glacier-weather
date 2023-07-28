import QtQuick
import Nemo.Controls

import "../components"

Page {
    id: mainPage

    headerTools: HeaderToolsLayout {
        id: tools
        title: qsTr("Weather")

        tools: [
            ToolButton {
                iconSource: "image://theme/map"
                onClicked: {
                    window.pageStack.push(Qt.resolvedUrl("PlacesPage.qml"), {settingsObject: settings})
                }
            }
        ]

        drawerLevels: [
            ButtonRow {
                anchors.margins: Theme.itemSpacingMedium
                currentIndex: settings.windUnits
                model: settings.speedUnitsOptions
                onCurrentIndexChanged: {
                    settings.windUnits = currentIndex;
                }
            },
            ButtonRow {
                anchors.margins: Theme.itemSpacingMedium
                currentIndex: settings.temperatureUnits
                model: settings.temperatureUnitsOptions
                onCurrentIndexChanged: {
                    settings.temperatureUnits = currentIndex
                }
            }
        ]
    }

    Item {
        id: main
        anchors.fill: parent

        Spinner {
            id: spinner
            enabled: !weatherModel.hasValidWeather
            anchors.centerIn: parent
        }

        Column {
            spacing: Theme.itemSpacingExtraSmall
            visible: weatherModel.hasValidWeather

            anchors {
                fill: parent
                margins: Theme.itemSpacingExtraSmall
            }

            Item {
                width: parent.width
                height: Theme.itemHeightHuge

                Text {
                    text: (weatherModel.hasValidWeather ? weatherModel.city : qsTr("Search location"))
                    anchors.fill: parent
                    anchors.margins: Theme.itemSpacingLarge

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: Theme.textColor
                    font.pixelSize: Theme.fontSizeExtraLarge

                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        settings.selectedCity = (settings.selectedCity + 1) % settings.places.count
                    }
                }
            }

            BigForecastIcon {
                id: current

                width: main.width -12

                height: 2 * (main.height - Theme.itemHeightHuge ) / 3 - 10

                weatherIcon: weatherModel.hasValidWeather ? weatherModel.get(0).weatherIcon : "01d"
                temperatureMin: weatherModel.hasValidWeather ? weatherModel.get(0).temperatureMin : 0
                temperatureMax: weatherModel.hasValidWeather ? weatherModel.get(0).temperatureMax : 0
                windDirection: (weatherModel.hasValidWeather ? weatherModel.get(0).windDirection : 0)
                windSpeed: (weatherModel.hasValidWeather ?  weatherModel.get(0).windSpeed : 0)
                bottomText: (weatherModel.hasValidWeather ? weatherModel.get(0).weatherDescription : qsTr("No weather data"))

                pressed: bigForecastIconMouse.pressed

                MouseArea {
                    id: bigForecastIconMouse;
                    anchors.fill: parent
                    onClicked: {
                        model.refreshWeather()
                    }
                }
            }

            Row {
                id: iconRow
                spacing: 6

                width: main.width - 12
                height: (main.height - Theme.itemHeightHuge) / 3

                property real iconWidth: iconRow.width / 4 - 10
                property real iconHeight: iconRow.height * 0.75

                ForecastIcon {
                    id: forecast1
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    timestamp: (weatherModel.hasValidWeather ? weatherModel.get(1).dateTime : "0")
                    temperatureMin: weatherModel.hasValidWeather ? weatherModel.get(1).temperatureMin : 0
                    temperatureMax: weatherModel.hasValidWeather ? weatherModel.get(1).temperatureMax : 0
                    weatherIcon: (weatherModel.hasValidWeather ?
                                      weatherModel.get(1).weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast2
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    timestamp: (weatherModel.hasValidWeather ? weatherModel.get(2).dateTime : "0")
                    temperatureMin: weatherModel.hasValidWeather ? weatherModel.get(2).temperatureMin : 0
                    temperatureMax: weatherModel.hasValidWeather ? weatherModel.get(2).temperatureMax : 0
                    weatherIcon: (weatherModel.hasValidWeather ?
                                      weatherModel.get(2).weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast3
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    timestamp: (weatherModel.hasValidWeather ? weatherModel.get(3).dateTime : "0")
                    temperatureMin: weatherModel.hasValidWeather ? weatherModel.get(3).temperatureMin : 0
                    temperatureMax: weatherModel.hasValidWeather ? weatherModel.get(3).temperatureMax : 0
                    weatherIcon: (weatherModel.hasValidWeather ?
                                      weatherModel.get(3).weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast4
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    timestamp: (weatherModel.hasValidWeather ? weatherModel.get(4).dateTime : "0")
                    temperatureMin: weatherModel.hasValidWeather ? weatherModel.get(4).temperatureMin : 0
                    temperatureMax: weatherModel.hasValidWeather ? weatherModel.get(4).temperatureMax : 0
                    weatherIcon: (weatherModel.hasValidWeather ?
                                      weatherModel.get(4).weatherIcon : "01d")
                }
            }
        }
    }
}
