import QtQuick 2.6

import QtQuick.Controls 1.0
import QtQuick.Controls.Nemo 1.0
import QtQuick.Controls.Styles.Nemo 1.0
import Nemo.Dialogs 1.0

import WeatherInfo 1.0

import "../components"

Page {
    id: mainPage

    property variant settingsObject

    headerTools: HeaderToolsLayout {
        id: tools
        title: qsTr("Places")
        showBackButton: true;
        tools: [
            ToolButton {
                iconSource: "image://theme/plus"
                onClicked: {
                    addDialog.open()
                    addTextField.forceActiveFocus()
                }
            }
        ]

    }

    Rectangle {
        anchors.fill: parent;
        color: "transparent"
    }


    CustomDialog {
        id: addDialog
        //        inline: false;
        icon: "image://theme/map-marked"
        headingText: qsTr("Add place")
        visible: false;
        acceptText: qsTr("Ok")
        cancelText: qsTr("Cancel")
        acceptEnabled: addTextField.text.length >= 2
        content: [
            TextField {
                id: addTextField
                placeholderText: qsTr("Enter name of city");
                width: parent.width - gpsIcon.width - Theme.itemSpacingLarge
                onAccepted: {
                    addDialog.accepted();
                }
            },
            NemoIcon {
                id: gpsIcon
                anchors.right: parent.right;
                height: addTextField.height
                width: height
                source: "image://theme/satellite"
                color:  gpsIconMouse.pressed ? Theme.accentColor : Theme.textColor
                MouseArea {
                    id: gpsIconMouse
                    anchors.fill:parent;
                    onClicked: {
                        addTextField.text = qsTr("Trying to get position ...")
                        placesModel.update();
                        addTextField.forceActiveFocus()

                    }
                }
            }

        ]

        onAccepted: {
            // TODO validator
            settingsObject.places.insert(0, {city: addTextField.text, useGps: false})
            addDialog.close()


        }
        onCanceled: {
            addDialog.close()
        }

    }

    Binding {
        target: addTextField
        property: "text"
        value: placesModel.city
    }

    ListView {
        anchors.fill: parent;
        model: settingsObject.places
        delegate: ListViewItemWithActions {
            label: city;
            icon: useGps ? "image://theme/satellite" : "image://theme/map-marker-alt"
            showNext: true
            actions:[
                ActionButton {
                    iconSource: "image://theme/trash"
                    visible: !useGps
                    onClicked: {
                        settingsObject.places.remove(index)
                    }
                }
            ]
            onClicked: {
                console.log("selected city " + city)
                settingsObject.selectedCity = index;
                mainPage.pageStack.pop();
            }

        }

    }


}
