/**
 * @file MainApp_L.qml
 * @brief Landscape main application UI for NextGen Display.
 *
 * This QML file defines the main layout for landscape orientation, organizing the
 * top bar, safety area, navigation bar, and main content area. It dynamically loads
 * the home screen or settings screen based on the application state.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
import QtQuick 2.12
import Styles 1.0
import "./Pages"
import "./Components"


Item {
    id: landscapeMain
    anchors.fill: parent

    /**
     * @brief Main horizontal layout row.
     *
     * Contains the top bar, safety area, main content loader, and navigation bar.
     */
    Row {
        anchors.fill: parent
        TopBar {
            id: topBarL
            height: parent.height
            width: parent.width * 0.1
        }
        SafetyArea {
            id: safetyAreaL
            height: parent.height
            width: parent.width * 0.1
        }

        /**
         * @brief Loader for main content area.
         *
         * Loads either the home screen or settings screen based on mainWindow.isHomescreen.
         */
        Loader {
            height: landscapeMain.height
            width: landscapeMain.width * 0.7
            sourceComponent: mainWindow.isHomescreen ? homescreenCompL : settingsCompL
        }
        NavigationBar {
            id: navigationBarL
            height: parent.height
            width: parent.width * 0.1
            onIsMenuSelectedChanged: {
                mainWindow.isHomescreen = isMenuSelected
            }
        }
    }

    /**
     * @brief Home screen component for landscape mode.
     */
    Component {
        id: homescreenCompL
        Row {
            GaugesArea {
                id: guageArea
                height: landscapeMain.height
                width: landscapeMain.width * 0.15
            }
            WidgetsArea {
                id: widgetArea
                height: landscapeMain.height
                width: landscapeMain.width * 0.55
            }
        }
    }

    /**
     * @brief Settings screen component for landscape mode.
     */
    Component {
        id: settingsCompL
        Menu {
            anchors.fill: parent
        }
    }
}
