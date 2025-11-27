import QtQuick 2.12
import Styles 1.0
import "./Pages"
import "./Components"

Item {
    id: landscapeMain
    anchors.fill: parent

    Row{
        anchors.fill: parent
        TopBar {
            id: topBarL
            height: parent.height
            width:  parent.width * 0.1
        }
        SafetyArea{
            id: safetyAreaL
            height: parent.height
            width:  parent.width * 0.1
        }
        Loader{
            height: landscapeMain.height
            width: landscapeMain.width * 0.7
            sourceComponent: mainWindow.isHomescreen ? homescreenCompL : settingsCompL
        }
        NavigationBar{
            id: navigationBarL
            height: parent.height
            width: parent.width * 0.1
            onIsMenuSelectedChanged: {
                mainWindow.isHomescreen = isMenuSelected
            }
        }
    }

    Component {
        id: homescreenCompL
        Row {
            GuagesArea{
                id: guageArea
                height: landscapeMain.height
                width: landscapeMain.width * 0.2
            }
            WidgetsArea{
                id: widgetArea
                height: landscapeMain.height
                width: landscapeMain.width * 0.5
            }
        }
    }

    Component {
        id: settingsCompL
        Row {
            Rectangle {
                width: 40
                height: 40
                radius: 6
                color: "red"
            }
        }
    }
}
