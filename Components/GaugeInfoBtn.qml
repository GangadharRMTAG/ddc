import QtQuick
import Styles 1.0

Rectangle{
    id:gaugeInfoBtn
    color: Styles.color.background

    property string sourceImg: ""
    property int indicatorPos: 0 // 0 = on left , 1 = on right , 2 = on both
    property int indicatorVal: 3 // 1 - 5 level of indicator

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
        return  Styles.color.textLight;
    }

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
