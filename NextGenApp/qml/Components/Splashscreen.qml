/**
 * @file Splashscreen.qml
 * @brief Splash screen component for CASE CONSTRUCTION UI.
 *
 * This QML file displays a centered logo on a styled background, used as the application's splash screen.
 */
import QtQuick 2.12
import Styles 1.0

Item {
    Rectangle{
        anchors.fill: parent
        color: Styles.color.flatRect

        /**
         * @brief Centered logo image.
         */
        Image {
            id: logo
            height: 80
            width: 240
            anchors.centerIn: parent
            source: "qrc:/Images/CASE_logo.png"
        }
    }
}
