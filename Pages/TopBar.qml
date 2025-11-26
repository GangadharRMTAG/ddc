import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {

    Rectangle{
        id:topbar
        Loader {
            id: orientationLoader
            active: true
            sourceComponent: isPortrait ? horizontalLayout : verticalLayout
        }
    }

    Component {
        id: horizontalLayout
        Row {
            spacing: 10
            Repeater {
                model: 8
                Rectangle {
                    width: 40
                    height: 40
                    color: "lightblue"
                    border.width: 1
                }
            }
        }
    }

    Component {
        id: verticalLayout
        Column {
            spacing: 10

            Repeater {
                model: 8
                Rectangle {
                    width: 40
                    height: 40
                    color: "lightgreen"
                    border.width: 1
                }
            }
        }
    }
}
