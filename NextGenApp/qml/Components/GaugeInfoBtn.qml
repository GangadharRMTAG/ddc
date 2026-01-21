/**
 * @file GaugeInfoBtn.qml
 * @brief Gauge info button component for NextGen Display UI.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @modified - Kunal Kokare
 */
import QtQuick
import Styles 1.0

Rectangle {
    id: gaugeInfoBtn
    color: Styles.color.background

    property string sourceImg: ""

    property int indicatorPos: 0

    property int indicatorVal: 1

    Image {
        id: gaugeInfoImg
        height: parent.height / 2
        width: parent.width / 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        source: sourceImg
    }

    Rectangle {
        id: statusBar
        height: parent.height * 0.1
        width: parent.width * 0.8
        radius: 20
        color: Styles.color.background

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: gaugeInfoImg.bottom
        anchors.topMargin: 1

        property int barCount: 8
        property real barGap: width * 0.01
        property real barWidth: (width - barGap * (barCount - 1)) / barCount


        property string leftEdgeSource: {
            if (indicatorPos === 0 || indicatorPos === 2 || indicatorPos === 3)
                return "qrc:/Images/GaugesArea/left_edge_red.svg"

            // indicatorPos === 1 || 4
            return "qrc:/Images/GaugesArea/left_edge_blue.svg"
        }
        property string rightEdgeSource: {
            if (indicatorPos === 1 || indicatorPos === 3 || indicatorPos === 4)
                return "qrc:/Images/GaugesArea/right_edge_red.svg"

            // indicatorPos === 0 || 2
            return "qrc:/Images/GaugesArea/middle_right.svg"
        }


        property bool showMiddleEdge:
            indicatorPos === 1 ||
            indicatorPos === 3 ||
            indicatorPos === 4

        /* ===================== LEFT EDGE ===================== */

        Image {
            id: leftBorder
            source: statusBar.leftEdgeSource

            x: -2
            width: statusBar.barWidth + 0.5
            height: parent.height * 1.5

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -height * 0.35
            z: 2
        }

        /* ===================== MIDDLE EDGE ===================== */


        Image {
            id: middleBorder
            visible: statusBar.showMiddleEdge
            source: "qrc:/Images/GaugesArea/middle.svg"

            x: leftBorder.x + leftBorder.width + statusBar.barGap


            width: {
                // stop BEFORE right edge starts
                var endX = rightBorder.x - statusBar.barGap
                return Math.max(0, endX - x)
            }

            height: parent.height * 0.25

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -parent.height * 0.50
            z: 2

        }



        /* ===================== RIGHT EDGE ===================== */

        Image {
            id: rightBorder
            source: statusBar.rightEdgeSource

            height: parent.height * 1.5

            x: (indicatorPos === 0 || indicatorPos === 2)
               ? leftBorder.x + leftBorder.width + statusBar.barGap
               : statusBar.width - width + 2

            width: (indicatorPos === 0 || indicatorPos === 2)
                   ? (statusBar.barWidth + statusBar.barGap) * statusBar.barCount
                     - statusBar.barGap
                     - x
                   : statusBar.barWidth + 0.5

            anchors.bottom: parent.bottom
            anchors.bottomMargin: -height * 0.35
            z: 2
        }


        /* ===================== INDICATOR BARS ===================== */

        Row {
            id: indicatorRow
            anchors.fill: parent
            spacing: statusBar.barGap
            z: 1

            Repeater {
                model: statusBar.barCount

                Rectangle {
                    width: statusBar.barWidth
                    height: parent.height
                    color: getIndicatorColor(index + 1)
                }
            }
        }
    }

    /* ===================== BAR COLOR LOGIC ===================== */

    function getIndicatorColor(index) {

        if (indicatorVal < index)
            return Styles.color.darkBackground

        switch (indicatorPos) {

            // Fuel
        case 0:
            if (indicatorVal === 1 && index === 1)
                return Styles.color.warningRed
            return Styles.color.valGreen

            // Engine Coolant
        case 1:
            if (indicatorVal === 1)
                return Styles.color.warningRed
            if (indicatorVal === 8)
                return Styles.color.warningRed
            return Styles.color.valGreen

            // Battery (both critical)
        case 2:
            if (indicatorVal === 1 || indicatorVal === 8)
                return Styles.color.warningRed
            return Styles.color.valGreen

            // DEF
        case 3:
            if (indicatorVal === 1 && index === 1)
                return Styles.color.warningRed
            if (indicatorVal === 8)
                return Styles.color.warningRed
            return Styles.color.valGreen

            // Hydraulic
        case 4:
            if (indicatorVal === 1)
                return Styles.color.warningRed
            if (indicatorVal === 8)
                return Styles.color.warningRed
            return Styles.color.valGreen

        default:
            return Styles.color.valGreen
        }
    }
}
