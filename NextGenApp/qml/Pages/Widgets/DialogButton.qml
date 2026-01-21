import QtQuick 2.15
import QtQuick.Controls 2.15
import Styles 1.0

Rectangle {
    id: root
    signal clicked()
    width: 100
    height: 100
    property color textColor: "#FFFFFF"
    property int textSize: 16
    property string text: ""
    radius: 7

    color:Styles.color.charcolNavy

    Text {
        text: root.text
        color: root.textColor
        font.pixelSize: root.textSize
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
        font.weight: Font.Medium
    }
    MouseArea{
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
