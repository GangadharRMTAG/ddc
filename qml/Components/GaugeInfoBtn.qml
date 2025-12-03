/**
 * @file GaugeInfoBtn.qml
 * @brief Gauge info button component for CASE CONSTRUCTION UI.
 *
 * This QML file provides a reusable gauge button with a status indicator bar, supporting
 * different indicator positions and critical state logic.
 */
import QtQuick
import Styles 1.0

Rectangle{
    id:gaugeInfoBtn
    color: Styles.color.background

    /**
     * @property sourceImg
     * @brief URL for the gauge image.
     */
    property string sourceImg: ""

    /**
     * @property indicatorPos
     * @brief Indicator position: 0 = left, 1 = right, 2 = both.
     */
    property int indicatorPos: 0

    /**
     * @property indicatorVal
     * @brief Indicator value (1-5 levels).
     */
    property int indicatorVal: 3


    /**
     * @brief Gauge image displayed at the top.
     */
    Image {
        id: gaugeInfoImg
        height: parent.height/2
        width: parent.width/4
        anchors{horizontalCenter: parent.horizontalCenter;top: parent.top;topMargin: height * 0.2}
        source: sourceImg
    }
    Rectangle{
        id: statusBar
        height: parent.height * 0.1
        width: parent.width * 0.8
        clip:true
        radius: 20
        anchors{horizontalCenter: parent.horizontalCenter;bottom: parent.bottom;bottomMargin: parent.height * 0.15}
        color: Styles.color.background
        Row{
            anchors.fill: parent
            spacing: parent.width * 0.01
            Repeater {
                model: 5
                Rectangle {
                    height: parent.height
                    width: parent.width * 0.19
                    color: getIndicatorColor(index + 1)
                }
            }
        }
    }

    /**
     * @brief Returns the color for each indicator bar based on value and position.
     * @param index The 1-based index of the indicator bar.
     * @return The color for the indicator bar.
     */
    function getIndicatorColor(index) {
        if (indicatorVal < index)
            return Styles.color.darkBackground;
        // Left side red
        if (indicatorPos === 0 && index === 1 && indicatorVal === 1)
            return Styles.color.warningRed;
        // Right side red
        if (indicatorPos === 1 && index === 5 && indicatorVal === 5)
            return Styles.color.warningRed;
        // Both sides red
        if (indicatorPos === 2 && (index === 1 || index === 5) && (indicatorVal === index))
            return Styles.color.warningRed;
        // Otherwise normal active color
        return Styles.color.textLight;
    }

    /**
     * @brief Returns true if the indicator is in a critical state.
     * @return True if critical, false otherwise.
     */
    function isCritical() {
        // Critical on LEFT
        if ((indicatorPos === 0 || indicatorPos === 2) && indicatorVal === 1)
            return true;
        // Critical on RIGHT
        if ((indicatorPos === 1 || indicatorPos === 2) && indicatorVal === 5)
            return true;
        return false;
    }
}
