/**
 * @file TripMenu.qml
 * @brief Main TripDetails/menu page for the application.
 *
 * This QML file implements the Trip Menu page which displays a header
 * and a list of TripDetails items. The list is backed by a ListModel and each
 * item is presented via a delegate that uses the project's FlatButton component.
 *
 * @date 23-Dec-2025
 * @author Sailee Shuddhalwar
 * @updated Kunal kokare , Sailee Shuddhalwar
 */
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Styles 1.0
import "../Components"
import ScreenUtils 1.0

Item {
    id: tripMenuRoot
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
    property int selectedIndex: 1

    Rectangle {
        id: menuBackground
        anchors.fill: parent
        color: Styles.color.background
        z: -1

        Rectangle {
            id: headerRect
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: ScreenUtils.scaledHeight(mainWindow.height, 85)
            color: Styles.color.background

            RowLayout {
                anchors.fill: parent
                spacing: ScreenUtils.scaledWidth(tripMenuRoot.width, 14)
                anchors.leftMargin: ScreenUtils.scaledWidth(tripMenuRoot.width, 14)
                anchors.rightMargin: ScreenUtils.scaledWidth(tripMenuRoot.width, 14)
                Image {
                    source: "qrc:/Images/TripScreen/speedMeterWhite.svg"
                    Layout.preferredWidth: ScreenUtils.scaledWidth(tripMenuRoot.width, 45)
                    Layout.preferredHeight: ScreenUtils.scaledWidth(tripMenuRoot.width, 45)

                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: {
                        if(selectedIndex === 1 ) return "Trip A";
                        else if(selectedIndex === 2 ) return "Trip B";
                        else return ""
                    }
                    font.pixelSize: Math.round(18 * settingsMenuRoot.uiScale)
                    font.weight: Font.Medium
                    color: "#ffffff"
                }

                Item{
                    Layout.fillWidth: true
                }

                Image {
                    source:(selectedIndex === 1 ) ? "qrc:/Images/TripScreen/left_arrow_inactive.svg" : "qrc:/Images/TripScreen/left_arrow_active.svg"
                    Layout.preferredWidth: ScreenUtils.scaledWidth(tripMenuRoot.width, 42)
                    Layout.preferredHeight: ScreenUtils.scaledWidth(tripMenuRoot.width, 42)
                    fillMode: Image.PreserveAspectFit
                    MouseArea{
                        anchors.fill: parent
                        onClicked: selectedIndex = 1
                    }
                }

                Text {
                    text: selectedIndex + "/2"
                    font.pixelSize: Math.round(18 * settingsMenuRoot.uiScale)
                    font.weight: Font.Medium
                    color: "#ffffff"
                }


                Image {
                    source:(selectedIndex === 2 ) ? "qrc:/Images/TripScreen/right_arrow_inactive.svg" : "qrc:/Images/TripScreen/right_arrow_active.svg"
                    Layout.preferredWidth: ScreenUtils.scaledWidth(tripMenuRoot.width, 42)
                    Layout.preferredHeight: ScreenUtils.scaledWidth(tripMenuRoot.width, 42)
                    fillMode: Image.PreserveAspectFit
                    MouseArea{
                        anchors.fill: parent
                        onClicked: selectedIndex = 2
                    }
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
            ListElement { title: "Trip Hours"; iconSource:  "qrc:/Images/TripScreen/tripHours.svg"; paramValue: "" }
            ListElement { title: "Fuel Usage"; iconSource: "qrc:/Images/TripScreen/fuelUsage.svg" ; paramValue:""}
            ListElement { title: "Fuel Rate"; iconSource: "qrc:/Images/TripScreen/fuelRate.svg" ; paramValue: ""}
            ListElement { title: "Def Usage"; iconSource: "qrc:/Images/TripScreen/defUsage.svg" ; paramValue: ""}
            ListElement { title: "Def Rate"; iconSource: "qrc:/Images/GaugesArea/DefLevel_W.svg" ; paramValue: ""}
            ListElement { title: "Average Engine % Load"; iconSource: "qrc:/Images/TripScreen/averageEnginePerLoad.svg" ; paramValue: ""}
        }

        Column {
            id: settingsListView
            anchors {
                left: parent.left
                right: parent.right
                top: headerRect.bottom
                bottom: resetAreaCl.top
            }
            Repeater{
                model: settingsModel
                delegate:Rectangle{
                    width: parent.width
                    height: ScreenUtils.scaledHeight(mainWindow.height, 85)
                    color:Styles.color.background

                    Row{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: horzSeperator.top
                        anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
                        anchors.topMargin: ScreenUtils.scaledHeight(mainWindow.height, 18)
                        anchors.bottomMargin: ScreenUtils.scaledHeight(mainWindow.height, 18)
                        spacing: ScreenUtils.scaledWidth(parent.width, 16)

                        Image {
                            source: model.iconSource
                            width: ScreenUtils.scaledHeight(mainWindow.height, 45)
                            height: ScreenUtils.scaledHeight(mainWindow.height, 45)
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Text {
                            text: model.title
                            color: Styles.color.lightGray
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                    }

                    Text {
                        id: paramValueTxt
                        text: {
                            switch (model.title) {
                            case "Fuel Usage":
                                return  appInterface.fuelUsage.toFixed(1) + " L"
                            case "Def Rate":
                                return appInterface.defRate.toFixed(1) + " L"
                            case "Fuel Rate":
                                return appInterface.fuelRate.toFixed(1) + " L/h"
                            case "Trip Hours":
                                return appInterface.tripHours.toFixed(1) + " h"

                            case "Def Usage" : return appInterface.defUsage.toFixed(1) + " L"
                            case "Average Engine % Load": return appInterface.avgEngineLoad + " %"
                            default:
                                return "--"
                            }
                        }
                        color: Styles.color.lightGray
                        Layout.alignment: Qt.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
                    }

                    Rectangle {
                        id: horzSeperator
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.left
                        anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
                        anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
                        height: 1
                        color:"#B1B3C4"

                    }

                }

            }

        }
        Column {
            id:resetAreaCl
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            //height: ScreenUtils.scaledHeight(mainWindow.height, 260)
            spacing: ScreenUtils.scaledHeight(mainWindow.height, 25)
            anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
            anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
            anchors.bottomMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)


            Text {
                id : lastResetText
                text: "Last Reset: " + appInterface.lastResetDate
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                color: "#CFCFCF"
                font.pixelSize: Math.round(14 * settingsMenuRoot.uiScale)
                font.styleName: "Normal"
                font.weight: Font.Medium
            }

            Rectangle {
                width:   ScreenUtils.scaledWidth(tripMenuRoot.width, 660)
                height: ScreenUtils.scaledHeight(mainWindow.height, 80)
                radius:  ScreenUtils.scaledWidth(tripMenuRoot.width, 16)
                color: Styles.color.popupBt1
                anchors.horizontalCenter: parent.horizontalCenter
                Row{
                    anchors.centerIn: parent
                    Image {
                        source: "qrc:/Images/TripScreen/reset.svg"
                        width: ScreenUtils.scaledWidth(tripMenuRoot.width, 64)
                        height: ScreenUtils.scaledWidth(tripMenuRoot.width, 64)
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                    }

                    Text {
                        text: "Reset"
                        color: Styles.color.popUpBg
                        font.pixelSize: Math.round(16 * settingsMenuRoot.uiScale)
                        font.weight: Font.Medium
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        appInterface.setLastTripHours(appInterface.engineHours)
                        appInterface.setFuelRate(0.0)
                        appInterface.setFuelUsage(0.0)
                        appInterface.setDefRate(0.0)
                        appInterface.setDefUsage(0.0)
                        appInterface.setAvgEngineLoad(0)


                        let d = new Date()
                        let mm = (d.getMonth()+1).toString().padStart(2,"0")
                        let dd = d.getDate().toString().padStart(2,"0")
                        let yy = d.getFullYear()

                        appInterface.setLastResetDate(mm + "/" + dd + "/" + yy)


                        console.log("RESET clicked, date set to", lastResetText.text)

                    }
                }
            }

            Item {
                height: 16 * settingsMenuRoot.uiScale
            }
        }

    }



}
