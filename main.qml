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

    property bool isHomescreen: true

    Splashscreen{
        id: mainSplashscreen
        z:99
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

    Timer{
        id: splashTimer
        interval: 1000
        repeat: false
        onTriggered: {
            mainSplashscreen.visible = false
        }
    }

}
