import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Styles 1.0
import "../Components"

Item {
    id: root
    ListModel {
        id: buttonModel
        ListElement { text: "H" }
        ListElement { text: "H" }
        ListElement { text: "H" }
        ListElement { text: "H" }
        ListElement { text: "H" }
        ListElement { text: "H" }
    }

    Loader {
        id: orientationLoader
        anchors.centerIn: parent
        active: true
        sourceComponent: isPortrait ? verticalLayout : horizontalLayout
    }

    Component {
        id: verticalLayout
        Row {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                SafetyAreaBtn {
                    text: model.text
                }
            }
        }
    }

    Component {
        id: horizontalLayout
        Column {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                SafetyAreaBtn {
                    text: model.text
                }
            }
        }
    }
}
