/**
 * @file GaugesArea.qml
 * @brief Gauges area for NextGen Display UI.
 *
 * This QML file provides a responsive area for displaying various machine gauges and indicators,
 * supporting both portrait and landscape layouts. Uses custom components for modularity.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @modified - Kunal Kokare
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0
import ScreenUtils 1.0
import "../Components"

Item {
    id: gaugesArea


    property int selectedIndex: 0
    property bool   centerModeActive: appInterface.creepActive
    property string centerStatusIcon: "qrc:/Images/SafetyArea/creep.svg"
    property string centerStatusText: appInterface.creepActive ? "57" : ""



    Rectangle {
        id: gaugesAreaRect
        anchors.fill: parent
        color: Styles.color.darkBackground

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
     * @brief Portrait layout for the gauges area.
     */
    Component {
        id: portraitLayout
        Rectangle {
            color: Styles.color.transparent
            Column{
                id: portraitRoot
                anchors.fill: parent
                anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
                anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
                Rectangle{
                    id : headerComponants
                    visible: selectedIndex !== 0
                    height: ScreenUtils.scaledHeight(portraitMain.height, 45)
                    width: parent.width
                    color: Styles.color.background

                    /* ================= TIME (LEFT) ================= */

                    Text {
                        id: timeText

                        text: appInterface.currentTime

                        width: ScreenUtils.scaledWidth(parent.width, 163)


                        x: ScreenUtils.scaledWidth(parent.width, 41)


                        anchors.top: parent.top
                        anchors.topMargin: parent.height * 0.15

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        font.pixelSize: 20

                        color: Styles.color.textLight
                    }



                    /* ================= ENGINE HOURS (RIGHT) ================= */

                    Text {
                        id: engineHoursText

                        width: ScreenUtils.scaledWidth(parent.width, 171)


                        x: ScreenUtils.scaledWidth(parent.width, 520)


                        anchors.top: parent.top
                        anchors.topMargin: parent.height * 0.15

                        text: appInterface.engineHours.toFixed(1) + " H"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        font.pixelSize: 20

                        color: Styles.color.textLight
                    }


                }

                Rectangle {
                    id: rpmHeaderComponant
                    visible: selectedIndex === 0

                    height: ScreenUtils.scaledHeight(portraitMain.height, 45)
                            + ScreenUtils.scaledHeight(portraitMain.height, 215)

                    width: parent.width
                    color: Styles.color.background

                    /* ================= LEFT ZONE (TIME) ================= */
                    Item {
                        width: ScreenUtils.scaledWidth(parent.width, 240)
                        height: parent.height

                        anchors.left: parent.left
                        anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)

                        Text {
                            text: timeText.text
                            anchors.centerIn: parent
                            font.pixelSize: 20
                            color: Styles.color.textLight
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    /* ================= CENTER ZONE (LOGO) ================= */
                    Item {
                        width: parent.width * 0.4
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter

                        Image {
                            source: "qrc:/Images/GaugesArea/Logo_Case.svg"
                            height: ScreenUtils.scaledHeight(portraitMain.height, 50)
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                        }
                    }

                    /* ================= RIGHT ZONE (ENGINE HOURS) ================= */
                    Item {
                        width: ScreenUtils.scaledWidth(parent.width, 260)
                        height: parent.height
                        anchors.right: parent.right
                        anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)

                        Text {
                            text: appInterface.engineHours.toFixed(1) + " H"
                            anchors.centerIn: parent
                            font.pixelSize: 20
                            font.bold: true
                            color: Styles.color.textLight
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                }


                Rectangle{
                    id:gaugeComponants
                    visible: selectedIndex !== 0
                    height: ScreenUtils.scaledHeight(portraitMain.height, 215)
                    width: parent.width
                    color: Styles.color.transparent
                    anchors.topMargin:  ScreenUtils.scaledHeight(parent.height, 400)
                    Grid{
                        anchors.fill: parent
                        columns: 3
                        rows: 2
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: appInterface.gauges[0] === 1
                                       ? "qrc:/Images/GaugesArea/FuelLevel_R.svg"
                                       : "qrc:/Images/GaugesArea/FuelLevel_W.svg"
                            indicatorPos: 0
                            indicatorVal: appInterface.gauges[0]
                        }
                        Rectangle{
                            height: parent.height/2
                            width: parent.width/3
                            color: Styles.color.background

                            Item {
                                id: centerCell
                                anchors.fill: parent

                                /* ================= RPM BLOCK (ALWAYS VISIBLE) ================= */

                                Column {
                                    id: rpmBlock

                                    width: parent.width
                                    height: parent.height
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter

                                    //Move RPM up when mode active
                                    anchors.verticalCenterOffset: gaugesArea.centerModeActive
                                                                  ? -parent.height * 0.40
                                                                  : 0

                                    spacing: -5

                                    Text {
                                        text: appInterface.rpm
                                        width: parent.width
                                        height: parent.height * 0.65
                                        color: Styles.color.lightGray
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: ScreenUtils.scaledHeight(mainWindow.height, 58)
                                        font.weight: Font.Bold
                                    }

                                    Text {
                                        text: "RPM"
                                        width: parent.width
                                        height: parent.height * 0.35
                                        color: "#C7C9D7"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: ScreenUtils.scaledHeight(mainWindow.height, 32)
                                        font.weight: Font.Medium
                                    }

                                    Behavior on anchors.verticalCenterOffset {
                                        NumberAnimation { duration: 180; easing.type: Easing.InOutQuad }
                                    }
                                }

                                /* ================= STATUS SLOT ================= */

                                Row {
                                    id: statusRow

                                    visible: gaugesArea.centerModeActive

                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: rpmBlock.bottom
                                    anchors.topMargin: parent.height * 0.02

                                    spacing: parent.width * 0.02

                                    Image {
                                        source: gaugesArea.centerStatusIcon
                                        height: 25
                                        width: height
                                        fillMode: Image.PreserveAspectFit
                                        anchors.verticalCenter: parent.verticalCenter
                                    }


                                    Text {
                                        visible: gaugesArea.centerStatusText !== ""
                                        text: gaugesArea.centerStatusText
                                        color: Styles.color.lightGray
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: ScreenUtils.scaledHeight(mainWindow.height, 28)
                                        font.weight: Font.Medium
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }


                        }

                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/EngineCoolant_W.svg"
                            indicatorPos: 1
                            indicatorVal: appInterface.gauges[1]
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/DefLevel_W.svg"
                            indicatorPos: 2
                            indicatorVal: appInterface.gauges[2]
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/BatteryVoltage_W.svg"
                            indicatorPos: 3
                            indicatorVal: appInterface.gauges[3]
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/HydraulicOil_W.svg"
                            indicatorPos: 4
                            indicatorVal: appInterface.gauges[4]
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Landscape layout for the gauges area.
     */
    Component {
        id: landscapeLayout
        Rectangle {
            anchors.fill: parent
            color: Styles.color.background
            Rectangle {
                width: parent.width
                height: parent.height * 0.9
                anchors.verticalCenter: parent.verticalCenter
                color: Styles.color.transparent
                Column{
                    anchors.fill: parent
                    Rectangle{
                        height: parent.height/6
                        width: parent.width
                        color: Styles.color.background
                        Image {
                            height: parent.height * 0.7
                            width: parent.width * 0.7
                            anchors.centerIn: parent
                            source: "qrc:/Images/CASE_logo.svg"
                        }
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: appInterface.gauges[0] === 1
                                   ? "qrc:/Images/GaugesArea/FuelLevel_R.svg"
                                   : "qrc:/Images/GaugesArea/FuelLevel_W.svg"
                        indicatorPos: 0
                        indicatorVal: appInterface.gauges[0]
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/EngineCoolant_W.svg"
                        indicatorPos: 1
                        indicatorVal: appInterface.gauges[1]
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/DefLevel_W.svg"
                        indicatorPos: 0
                        indicatorVal: appInterface.gauges[2]
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/BatteryVoltage_W.svg"
                        indicatorPos: 2
                        indicatorVal: appInterface.gauges[3]
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/HydraulicOil_W.svg"
                        indicatorPos: 1
                        indicatorVal: appInterface.gauges[4]
                    }
                }
            }
        }
    }
}
