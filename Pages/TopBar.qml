/**
 * @file TopBar.qml
 * @brief Top bar indicator area for CASE CONSTRUCTION UI.
 *
 * This QML file provides a responsive top bar with status icons, supporting both
 * portrait and landscape layouts. Icons are dynamically generated from a model.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0

Item {
    id: topBar

    /**
     * @brief Model for top bar status icons.
     *
     * Each element contains:
     *   - icon: Icon image URL
     */
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/TopBar/stop.png" }
        ListElement { icon: "qrc:/Images/TopBar/cautation.png" }
        ListElement { icon: "qrc:/Images/TopBar/seatBelt.png" }
        ListElement { icon: "qrc:/Images/TopBar/parkBrake.png" }
        ListElement { icon: "qrc:/Images/TopBar/workLamp.png" }
        ListElement { icon: "qrc:/Images/TopBar/beacon.png" }
        ListElement { icon: "qrc:/Images/TopBar/regeneration.png" }
        ListElement { icon: "qrc:/Images/TopBar/greedHeater.png" }
        ListElement { icon: "qrc:/Images/TopBar/hydraulicLock.png" }
        ListElement { icon: "qrc:/Images/TopBar/footPedal.png" }
    }

    Rectangle {
        id: topbarRect
        anchors.fill: parent
        color: Styles.color.transparent

        /**
         * @brief Loader for switching between portrait and landscape layouts.
         *
         * Loads the appropriate layout based on the isPortrait property.
         */
        Loader {
            id: orientationLoader
            anchors.centerIn: parent
            active: true
            sourceComponent: isPortrait ? portraitLayout : landscapeLayout
        }
    }


    /**
     * @brief Portrait layout for the top bar.
     */
    Component {
        id: portraitLayout

        Row {
            anchors.centerIn: parent

            Repeater {
                model: buttonModel
                Rectangle {
                    height: topbarRect.height
                    width: topbarRect.width/10
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        source: model.icon
                        width: parent.width * 0.8
                        height: parent.height * 0.8
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }


    /**
     * @brief Landscape layout for the top bar.
     */
    Component {
        id: landscapeLayout

        Column {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                Rectangle {
                    height: topbarRect.height/10
                    width: topbarRect.width
                    radius: 6
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        source: model.icon
                        width: parent.width * 0.8
                        height: parent.height * 0.8
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
