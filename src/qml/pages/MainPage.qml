import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0

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
                    mainPage.Stack.view.push(Qt.resolvedUrl("PlacesPage.qml"), {settingsObject: settings})
                }
            },
            ToolButton {
                iconSource: "image://theme/cog"
                onClicked: {
                    mainPage.Stack.view.push(Qt.resolvedUrl("SettingsPage.qml"), {settingsObject: settings})
                }
            }

        ]
    }

    WeatherSettings {
        id: settings;
        onSelectedCityChanged: updateWeatherModel();
        onPlacesModelChanged:  updateWeatherModel();

        function updateWeatherModel() {
            var item = settings.places.get(settings.selectedCity);
            if (!item.useGps) {
                model.city = item.city;
            }

            model.useGps = item.useGps;
        }
    }



    Item {
        id: main
        anchors.fill: parent

        Column {
            spacing: Theme.itemSpacingExtraSmall

            anchors {
                fill: parent
                margins: Theme.itemSpacingExtraSmall
            }

            Rectangle {
                width: parent.width
                height: Theme.itemHeightHuge
                color: "transparent"

                Text {
                    text: (model.hasValidCity ? model.city : "Unknown location") + (model.useGps ? " (GPS)" : "")
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

                weatherIcon: (model.hasValidWeather
                          ? model.weather.weatherIcon
                          : "01d")                

                tempText: (model.hasValidWeather
                          ? model.weather.temperature
                          : "??")

                windDirectionText: (model.hasValidWeather
                           ? model.weather.windDirection + "° "
                           : "")
                windSpeedText:(model.hasValidWeather
                               ?  model.weather.windSpeed + " m/s"
                               : "")

                bottomText: (model.hasValidWeather
                             ? model.weather.weatherDescription
                             : qsTr("No weather data"))

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

                    topText: (model.hasValidWeather ?
                              model.forecast[0].dayOfWeek : "??")
                    bottomText: (model.hasValidWeather ?
                                 model.forecast[0].temperature : "??/??")
                    weatherIcon: (model.hasValidWeather ?
                              model.forecast[0].weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast2
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    topText: (model.hasValidWeather ?
                              model.forecast[1].dayOfWeek : "??")
                    bottomText: (model.hasValidWeather ?
                                 model.forecast[1].temperature : "??/??")
                    weatherIcon: (model.hasValidWeather ?
                              model.forecast[1].weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast3
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    topText: (model.hasValidWeather ?
                              model.forecast[2].dayOfWeek : "??")
                    bottomText: (model.hasValidWeather ?
                                 model.forecast[2].temperature : "??/??")
                    weatherIcon: (model.hasValidWeather ?
                              model.forecast[2].weatherIcon : "01d")
                }
                ForecastIcon {
                    id: forecast4
                    width: iconRow.iconWidth
                    height: iconRow.iconHeight

                    topText: (model.hasValidWeather ?
                              model.forecast[3].dayOfWeek : "??")
                    bottomText: (model.hasValidWeather ?
                                 model.forecast[3].temperature : "??/??")
                    weatherIcon: (model.hasValidWeather ?
                              model.forecast[3].weatherIcon : "01d")
                }

            }
        }
    }



}
