// TopBar.qml
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0

Item {
    id: root
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/TopBar/stop.png" }
        ListElement { icon: "qrc:/Images/TopBar/cautation.png" }
        ListElement { icon: "qrc:/Images/TopBar/seatBelt.png" }
        ListElement { icon: "qrc:/Images/TopBar/parkBrake.png" }
        ListElement { icon: "qrc:/Images/TopBar/workLamp.png" }
        ListElement { icon: "qrc:/Images/TopBar/beacon.png" }
        ListElement { icon: "qrc:/Images/TopBar/regeneration.png" }
        ListElement { icon: "qrc:/Images/TopBar/greedHeater.png" }
        ListElement { icon: "qrc:/Images/TopBar/hydraulicLock.png" }
        ListElement { icon: "qrc:/Images/TopBar/footPedal.png" }
    }

    Rectangle {
        id: topbar
        anchors.fill: parent
        color: Styles.color.transparent

        Loader {
            id: orientationLoader
            anchors.centerIn: parent
            active: true
            sourceComponent: isPortrait ? potraitLayout : landscapeLayout
        }
    }

    Component {
        id: potraitLayout

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
        id: landscapeLayout

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
