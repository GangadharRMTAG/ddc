import QtQuick 2.12
import Styles 1.0
import "./Pages"
import "./Components"

Item {
    id: portraitMain
    anchors.fill: parent

    Column{
        anchors.fill: parent
        TopBar {
            id: topBarP
            height: portraitMain.height * 0.1
            width: portraitMain.width
        }
        SafetyArea{
            id: safetyAreaP
            height:  portraitMain.height * 0.1
            width:   portraitMain.width
        }
        Loader{
            height: portraitMain.height * 0.7
            width: portraitMain.width
            sourceComponent: mainWindow.isHomescreen ? homescreenCompP : settingsCompP
        }
        NavigationBar{
            id: navigationBarP
            height: parent.height * 0.1
            width: parent.width
            onIsMenuSelectedChanged: {
                mainWindow.isHomescreen = isMenuSelected
            }
        }
    }

    Component {
        id: homescreenCompP
        Column {
            GuagesArea{
                id: guageArea
                height: portraitMain.height * 0.2
                width: portraitMain.width
            }
            WidgetsArea{
                id: widgetArea
                height: portraitMain.height * 0.5
                width: portraitMain.width
            }
        }
    }

    Component {
        id: settingsCompP
        Column {
            Rectangle {
                width: 40
                height: 40
                radius: 6
                color: "red"
            }
        }
    }
}
