/**
 * @file TripInfo.qml
 * @brief Trip information widget for NextGen Display UI.
 *
 * This QML file provides a widget for displaying trip-related information.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @updated Kunal Kokare , Sailee Shuddhalwar
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ScreenUtils 1.0
import Styles 1.0

Item{
    id: tripInfo
    anchors.fill: parent


    GridLayout {
        id: grid
        anchors.fill: parent
        columns: 2
        rowSpacing: 0
        columnSpacing: 0

        Repeater {
            model: [
                { icon: "qrc:/Images/TripScreen/tripHours.svg", value: appInterface.tripHours.toFixed(1) + " h", unit: "" },
                { icon: "qrc:/Images/TripScreen/averageEnginePerLoad.svg", value: appInterface.avgEngineLoad + " %", unit: "" },
                { icon: "qrc:/Images/TripScreen/fuelUsage.svg", value: appInterface.fuelUsage.toFixed(1) + " L", unit: "" },
                { icon: "qrc:/Images/TripScreen/defUsage.svg", value: appInterface.defUsage.toFixed(1)+ " L", unit: "" },
                { icon: "qrc:/Images/TripScreen/fuelRate.svg", value: appInterface.fuelRate.toFixed(1) +" L/h", unit: "" },
                { icon: "qrc:/Images/GaugesArea/DefLevel_W.svg", value:appInterface.defRate.toFixed(1) + " L", unit: "" }
            ]

            delegate: Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Styles.color.transparent


                property int row: Math.floor(index / 2)
                property int col: index % 2

                // Right separator
                Rectangle {
                    visible: col < 1
                    width: 2
                    color: Styles.color.tripInfoSeparator
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                }

                // Bottom separator
                Rectangle {
                    visible: row < 2
                    height: 2
                    color: Styles.color.tripInfoSeparator
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
                Row {
                    anchors.centerIn: parent
                    spacing: 14

                    Image {
                        width: 28
                        height: 28
                        source: modelData.icon
                        fillMode: Image.PreserveAspectFit
                    }

                    Column {
                        spacing: 4

                        Text {
                            text: modelData.value
                            font.pixelSize: 18
                            font.bold: true
                            color: "white"
                        }

                        Text {
                            text: modelData.unit
                            font.pixelSize: 10
                            color: "#b0b0b0"
                            visible: modelData.unit !== ""
                        }
                    }
                }
            }
        }
    }


}
