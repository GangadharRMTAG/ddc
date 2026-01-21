/**
 * @file Menu.qml
 * @brief Main settings/menu page for the application.
 *
 * This QML file implements the application's Menu page which displays a header
 * and a list of settings/menu items. The list is backed by a ListModel and each
 * item is presented via a delegate that uses the project's FlatButton component.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Styles 1.0
import "../Components"
import ScreenUtils 1.0

Item {
    id: settingsMenuRoot
    anchors.fill: parent


    /**
     * @property designWidth
     * @brief Reference width for UI scaling.
     */
    property real designWidth: 360

    /**
     * @property uiScale
     * @brief UI scale factor based on current width.
     */
    property real uiScale: Math.max(0.5, settingsMenuRoot.width / designWidth)

    /**
     * @property selectedIndex
     * @brief Index of the currently selected menu item.
     */
    property int selectedIndex: -1

    Rectangle {
        id: menuBackground
        anchors.fill: parent
        color: Styles.color.background
        z: -1

        Rectangle{
            id: gaugeIndicatorRect
            color : Styles.color.darkBackground
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: ScreenUtils.scaledHeight(mainWindow.height, 104)
            Row{
                anchors.fill: parent
                anchors.bottomMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
                GaugeInfoBtn{
                    height: parent.height
                    width: parent.width/3
                    color : Styles.color.darkBackground
                    sourceImg: "qrc:/Images/GaugesArea/FuelLevel.svg"
                    indicatorPos: 0
                    indicatorVal: appInterface.gauges[0]
                }

                Item{
                    height: parent.height
                    width: parent.width/3
                    Column{
                        anchors.fill: parent
                        spacing: 0
                        Text {
                            text: appInterface.rpm * 1000
                            width: parent.width
                            height: parent.height/2
                            color: Styles.color.lightGray
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: ScreenUtils.scaledHeight(mainWindow.height, 74)
                            font.styleName: "Normal"
                            font.weight: Font.DemiBold
                        }
                        Text {
                            text: "RPM"
                            width: parent.width
                            height: parent.height/2
                            color: "#C7C9D7"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: ScreenUtils.scaledHeight(mainWindow.height, 32)
                            font.styleName: "Normal"
                            font.weight: Font.Medium
                        }
                    }
                }
                GaugeInfoBtn{
                    height: parent.height
                    width: parent.width/3
                    color : Styles.color.darkBackground
                    sourceImg: "qrc:/Images/GaugesArea/DefLevel_W.svg"
                    indicatorPos: 2
                    indicatorVal: appInterface.gauges[2]
                }
            }

        }

        Rectangle {
            id: headerRect
            anchors {
                left: parent.left
                right: parent.right
                top: gaugeIndicatorRect.bottom
            }
            height: 56 * settingsMenuRoot.uiScale
            color: Styles.color.background

            Row {
                anchors.fill: parent
                anchors.leftMargin: 16 * settingsMenuRoot.uiScale
                spacing: 12 * settingsMenuRoot.uiScale

                Rectangle {
                    width: 24 * settingsMenuRoot.uiScale
                    height: 24 * settingsMenuRoot.uiScale
                    color: Styles.color.transparent
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        anchors.centerIn: parent
                        spacing: 4 * settingsMenuRoot.uiScale
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                    }
                }

                Text {
                    text: "Menu"
                    font.pixelSize: Math.round(18 * settingsMenuRoot.uiScale)
                    font.weight: Font.Medium
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }


        /**
         * @brief Model for settings menu items.
         *
         * Each element contains:
         *   - title: Menu item title
         *   - iconSource: Icon image URL
         */
        ListModel {
            id: settingsModel
            ListElement { title: "Machine Service"; iconSource:  "qrc:/Images/SettingsPage/machineService.svg" }
            ListElement { title: "Machine Settings"; iconSource: "qrc:/Images/SettingsPage/machineSettings.svg" }
            ListElement { title: "Camera Settings"; iconSource: "qrc:/Images/SettingsPage/cameraSettings.svg" }
            ListElement { title: "Trip Information"; iconSource: "qrc:/Images/SettingsPage/tripInformation.svg" }
            ListElement { title: "Machine Status"; iconSource: "qrc:/Images/SettingsPage/machineStatus.svg" }
            ListElement { title: "Phone"; iconSource: "qrc:/Images/SettingsPage/phone.svg" }
            ListElement { title: "Lights"; iconSource: "qrc:/Images/SettingsPage/lights.svg" }
            ListElement { title: "Radio"; iconSource: "qrc:/Images/SettingsPage/radio.svg" }
            ListElement { title: "Climate"; iconSource: "qrc:/Images/SettingsPage/climate.svg" }

        }

        ListView {
            id: settingsListView
            anchors {
                left: parent.left
                right: parent.right
                top: headerRect.bottom
                bottom: parent.bottom
            }
            model: settingsModel
            spacing: 0
            clip: true

            delegate: Column {
                width: settingsListView.width
                spacing: 0

                FlatButton {
                    title: model.title
                    iconSource: model.iconSource
                    isArrow: true
                    onClicked: {
                        selectedIndex = index
                        if(title === "Trip Information"){
                            tripScreenLoader.source = "qrc:/Pages/TripInfoScreen.qml"
                            headerRect.visible = false;
                        }
                        console.log("Clicked:", title, "selectedIndex:", index)
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                width: Math.max(3, Math.round(5 * settingsMenuRoot.uiScale))
                policy: ScrollBar.AsNeeded
                contentItem: Rectangle {
                    radius: height / 2
                    color: "#5a6270"
                    opacity: scrollBar.active || scrollBar.hovered ? 0.9 : 0.6
                    Behavior on opacity { NumberAnimation { duration: 200 } }
                }
                background: Rectangle { color: Styles.color.transparent }
                anchors.right: parent.right
                anchors.rightMargin: Math.round(2 * settingsMenuRoot.uiScale)
            }

        }
        Loader {
            id: tripScreenLoader
            anchors {
                left: parent.left
                right: parent.right
                top: gaugeIndicatorRect.bottom
                bottom: parent.bottom
            }
        }

    }

}


