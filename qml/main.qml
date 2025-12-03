/**
 * @file main.qml
 * @brief Main entry point for the CASE CONSTRUCTION application UI.
 *
 * This QML file sets up the main application window, handles orientation-based layout loading,
 * and displays a splash screen on startup.
 */
import QtQuick 2.12
import QtQuick.Window 2.12
import Styles 1.0
import "./Components"

Window {
    id:mainWindow
    width: isPortrait ? 400 : 850
    height:isPortrait ? 620 : 450
    visible: true
    color : Styles.color.charcolNavy
    title: qsTr("CASE CONSTRUCTION")

    /**
     * @property isHomescreen
     * @brief Indicates if the current screen is the home screen.
     */
    property bool isHomescreen: true

    /**
     * @brief Splash screen overlay displayed on startup.
     */
    Splashscreen {
        id: mainSplashscreen
        z: 99
        anchors.fill: parent
    }
    Loader{
        anchors.fill: parent
        source: isPortrait ? "qrc:/MainApp_P.qml" : "qrc:/MainApp_L.qml"
    }

    Component.onCompleted: {
        console.log("Component.onCompleted in main.qml Start")
        splashTimer.start()
        console.log("Component.onCompleted in main.qml End")
    }

    /**
     * @brief Timer to hide the splash screen after a delay.
     */
    Timer {
        id: splashTimer
        interval: 1000
        repeat: false
        onTriggered: {
            mainSplashscreen.visible = false
        }
    }
}
