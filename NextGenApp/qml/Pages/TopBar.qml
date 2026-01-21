/**
* @file TopBar.qml
* @brief Top bar indicator area for NextGen Display UI.
*
* This QML file provides a responsive top bar with status icons, supporting both
* portrait and landscape layouts. Icons are dynamically generated from a model.
*
* @date 08-Dec-2025
* @author Gangadhar Thalange

 * Updated
 * @date 18-Dec-2025
 * @author Kunal Kokare
*/
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0
import ScreenUtils 1.0

Item {
    id: topBar

    property var telltale: appInterface.telltales

    /**
     * @brief Model for top bar status icons.
     *
     * Each element contains:
     *   - icon: Icon image URL
     */
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/TopBar/stop.svg" }
        ListElement { icon: "qrc:/Images/TopBar/cautation.svg" }
        ListElement { icon: "qrc:/Images/TopBar/seatBelt.svg" }
        ListElement { icon: "qrc:/Images/TopBar/parkBrake.svg" }
        ListElement { icon: "qrc:/Images/TopBar/workLamp.svg" }
        ListElement { icon: "qrc:/Images/TopBar/beacon.svg" }
        ListElement { icon: "qrc:/Images/TopBar/regeneration.svg" }
        ListElement { icon: "qrc:/Images/TopBar/greedHeater.svg" }
        ListElement { icon: "qrc:/Images/TopBar/hydraulicLock.svg" }
        ListElement { icon: "qrc:/Images/TopBar/footPedal.svg" }
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
            anchors.fill: parent
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
            anchors.fill: parent
            anchors.leftMargin: ScreenUtils.scaledWidth(topbarRect.width, 14)
            anchors.rightMargin: ScreenUtils.scaledWidth(topbarRect.width, 14)
            spacing: ScreenUtils.scaledWidth(topbarRect.width, 7)
            Repeater {
                model: buttonModel
                Rectangle {
                    height:ScreenUtils.scaledHeight(topbarRect.height)
                    width: ScreenUtils.scaledWidth(topbarRect.width, 63)
                    color: "Transparent"
                    Image {
                        anchors.centerIn: parent
                        source: model.icon
                        visible: telltale[index] === 1
                        width: ScreenUtils.scaledWidth(topbarRect.width, 50)
                        height: ScreenUtils.scaledWidth(topbarRect.width, 50)
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
                        visible: telltale[index] === 1
                        width: parent.width * 0.8
                        height: parent.height * 0.8
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
