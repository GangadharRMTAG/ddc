import QtQuick
import Styles 1.0

Text {
//    anchors { left: parent.left; right: parent.right;
//        verticalCenter: parent.verticalCenter; margins: Styles.item.leftMargin }
//    height: Styles.item.height
    property bool hasTooltip: true
    font.family: Styles.font.awesome_6_pro
    font.pixelSize: Styles.fontSize.item
    color: Styles.theme.pureBlack//Styles.color.textDark
    elide: Text.ElideRight
    minimumPixelSize: 14
    fontSizeMode: Text.Fit
    verticalAlignment: Text.AlignVCenter
    Loader {
        sourceComponent: toolTipComponent
        anchors.fill: parent
    }
    Component {
        id: toolTipComponent
        Item {
            anchors.fill: parent
            MouseArea {
                anchors {fill: parent}
                onPressAndHold: {
                }
            }
        }
    }
}
