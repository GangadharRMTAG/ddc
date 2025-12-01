import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0

Rectangle {
    id: flatBtn
    width: parent ? parent.width : 360
    height: rowItem.height + divider.height

    property string title: ""
    property string iconSource: ""
    property bool isArrow: true
    signal clicked()

     property color bgNormal:  "#2d3249"
     property color bgHover:   "#32384f"
     property color bgPressed: "#374058"
     property color dividerColor: "#ffffff"
     property real  designWidth: 360

    property real uiScale: {
        var s = flatBtn.width / designWidth;
        if (s < 0.6) s = 0.6;
        if (s > 1.2) s = 1.2;   // max 1.2x
        return s;
    }

    color: bgNormal

    Rectangle {
        id: rowItem
        width: parent.width
        height: Math.round(56 * flatBtn.uiScale)
        color: bgNormal

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: rowItem.color = bgHover
            onExited:  rowItem.color = bgNormal
            onPressed: rowItem.color = bgPressed
            onReleased: {
                rowItem.color = bgHover
                flatBtn.clicked()
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16 * flatBtn.uiScale
            anchors.rightMargin: 16 * flatBtn.uiScale
            spacing: 10 * flatBtn.uiScale

            Image {
                id: iconImg
                source: flatBtn.iconSource

                width: Math.round(22 * flatBtn.uiScale)
                height: Math.round(22 * flatBtn.uiScale)

                sourceSize.width: width
                sourceSize.height: height

                fillMode: Image.PreserveAspectFit
                smooth: true
                visible: flatBtn.iconSource !== ""
                Layout.alignment: Qt.AlignVCenter
            }

            Text {
                id: txt
                text: flatBtn.title
                font.pixelSize: Math.round(16 * flatBtn.uiScale)
                font.weight: Font.Bold
                color: "#E0E0E0"
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                leftPadding: 8 * flatBtn.uiScale
            }

            Text {
                visible: flatBtn.isArrow
                text: Styles.text.arrow
                font.pixelSize: Math.round(20 * flatBtn.uiScale)
                color: "white"
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: Math.round(12 * flatBtn.uiScale)
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    Rectangle {
        id: divider
        width: parent.width
        height: Math.max(1, Math.round(1 * flatBtn.uiScale))
        color: dividerColor
        opacity: 0.06
        anchors.bottom: parent.bottom
    }
}
