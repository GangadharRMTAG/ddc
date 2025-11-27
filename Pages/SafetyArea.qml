import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Styles 1.0
import "../Components"

Item {
    id: root
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/SafetyArea/iso.png";   selectedIcon: "qrc:/Images/SafetyArea/H.png" }
        ListElement { icon: "qrc:/Images/SafetyArea/bdsl.png";  selectedIcon: "qrc:/Images/SafetyArea/bdsl.png" }
        ListElement { icon: "qrc:/Images/SafetyArea/creep.png"; selectedIcon: "qrc:/Images/SafetyArea/creep.png" }
        ListElement { icon: "" ;                                selectedIcon: ""}
        ListElement { icon: "" ;                                selectedIcon: ""}
        ListElement { icon: "" ;                                selectedIcon: ""}
    }

    Loader {
        id: orientationLoader
        anchors.centerIn: parent
        active: true
        sourceComponent: isPortrait ?  potraitLayout : landscapeLayout
    }

    Component {
        id: potraitLayout
        Row {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                SafetyButton {
                    source:  selected ? model.selectedIcon :  model.icon
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
                SafetyButton {
                    source:  selected ? model.selectedIcon :  model.icon
                }
            }
        }
    }
}
