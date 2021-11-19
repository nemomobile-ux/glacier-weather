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
                currentIndex: settingsObject.imperialUnits ? 0 : 1
                model: ListModel {
                    ListElement {
                        name: qsTr("Celsius")
                    }
                    ListElement {
                        name: qsTr("Fahrenheit")
                    }
                }
                onCurrentIndexChanged: {
                    settingsObject.imperialUnits = (currentIndex == 0)
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
                model: ListModel {
                    ListElement {
                        name: qsTr("Mph")
                    }
                    ListElement {
                        name: qsTr("m/s")
                    }
                    ListElement {
                        name: qsTr("km/h")
                    }
                    ListElement {
                        name: qsTr("knots")
                    }
                }
                onCurrentIndexChanged: {
                    settingsObject.windUnits = currentIndex;
                }
            }
        }

    }

}
