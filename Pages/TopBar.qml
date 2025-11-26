// TopBar.qml
import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/stop.png" }
        ListElement { icon: "qrc:/Images/cautation.png" }
        ListElement { icon: "qrc:/Images/seatBelt.png" }
        ListElement { icon: "qrc:/Images/parkBrake.png" }
        ListElement { icon: "qrc:/Images/workLamp.png" }
        ListElement { icon: "qrc:/Images/beacon.png" }
        ListElement { icon: "qrc:/Images/regeneration.png" }
        ListElement { icon: "qrc:/Images/greedHeater.png" }
        ListElement { icon: "qrc:/Images/hydraulicLock.png" }
        ListElement { icon: "qrc:/Images/footPedal.png" }
    }

    Rectangle {
        id: topbar
        anchors.fill: parent
        color: "transparent"

        Loader {
            id: orientationLoader
            anchors.centerIn: parent
            active: true
            sourceComponent: isPortrait ? horizontalLayout : verticalLayout
        }
    }

    Component {
        id: horizontalLayout

        Row {
            anchors.centerIn: parent

            Repeater {
                model: buttonModel
                Rectangle {
                    width: 40
                    height: 40
                    radius: 6
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        source: model.icon
                        width: 40
                        height: 40
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }

    Component {
        id: verticalLayout

        Column {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                Rectangle {
                    width: 40
                    height: 40
                    radius: 6
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        source: model.icon
                        width: 40
                        height: 40
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
