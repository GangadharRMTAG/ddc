/**
 * @file MainApp_P.qml
 * @brief Portrait main application UI for CASE CONSTRUCTION.
 *
 * This QML file defines the main layout for portrait orientation, organizing the
 * top bar, safety area, navigation bar, and main content area. It dynamically loads
 * the home screen or settings screen based on the application state.
 */
import QtQuick 2.12
import Styles 1.0
import "./Pages"
import "./Components"

Item {
    id: portraitMain
    anchors.fill: parent

    /**
     * @brief Main vertical layout column.
     *
     * Contains the top bar, safety area, main content loader, and navigation bar.
     */
    Column {
        anchors.fill: parent
        TopBar {
            id: topBarP
            anchors.horizontalCenter: parent.horizontalCenter
            height: portraitMain.height * 0.1
            width: portraitMain.width * 0.98
        }
        SafetyArea{
            id: safetyAreaP
            height:  portraitMain.height * 0.1
            width:   portraitMain.width
        }
        /**
         * @brief Loader for main content area.
         *
         * Loads either the home screen or settings screen based on mainWindow.isHomescreen.
         */
        Loader {
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

    /**
     * @brief Home screen component for portrait mode.
     */
    Component {
        id: homescreenCompP
        Column {
            GaugesArea{
                id: gaugeArea
                height: portraitMain.height * 0.2
                width: portraitMain.width
            }
            WidgetsArea{
                id: widgetArea
                anchors.horizontalCenter: parent.horizontalCenter
                height: portraitMain.height * 0.5
                width: portraitMain.width * 0.95
            }
        }
    }

    /**
     * @brief Settings screen component for portrait mode.
     */
    Component {
        id: settingsCompP
        Menu {
            anchors.fill: parent
        }
    }
}
