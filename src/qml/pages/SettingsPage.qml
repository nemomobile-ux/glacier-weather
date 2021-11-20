import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0

import "../components"

Page {
    id: mainPage

    property variant settingsObject

    headerTools: HeaderToolsLayout {
        id: tools
        title: qsTr("Settings")
        showBackButton: true;

    }


    Column {
        anchors.fill: parent;

        Rectangle {
            color: "transparent"
            width: parent ? parent.width : 0
            height: Theme.itemHeightHuge


            ButtonRow {
                anchors.margins: Theme.itemSpacingMedium
                currentIndex: settingsObject.temperatureUnits
                model: settingsObject.temperatureUnitsOptions
                onCurrentIndexChanged: {
                    settingsObject.temperatureUnits = currentIndex
                }
            }
        }


        Rectangle {
            color: "transparent"
            width: parent ? parent.width : 0
            height: Theme.itemHeightHuge


            ButtonRow {
                anchors.margins: Theme.itemSpacingMedium
                currentIndex: settingsObject.windUnits
                model: settingsObject.speedUnitsOptions
                onCurrentIndexChanged: {
                    settingsObject.windUnits = currentIndex;
                }
            }
        }

    }

}
