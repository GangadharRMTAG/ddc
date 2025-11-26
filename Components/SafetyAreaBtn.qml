import QtQuick
import QtQuick.Controls
import Styles 1.0
import "../Components"

Item {
    id: root
    width: 65
    height: 65

    property alias text: label.text
    property bool selected: false
    signal clicked()
    property bool barOn: false

    Rectangle {
        id: tile
        anchors.margins: 2
        anchors.fill: parent
        radius: 6
        color: "#3b3f56"
    }

    AppText {
        id: label
        text: "H"
        color: Styles.color.lightBackground
        font.pixelSize: 30
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        id: progressBar
        width: 50
        height: 6
        radius: height / 2
        color: root.barOn ?Styles.color.lightBackground: Styles.color.pureBlack
        anchors{ horizontalCenter:parent.horizontalCenter; bottom:parent.bottom; bottomMargin:7 }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: {
            root.barOn = !root.barOn
            root.clicked()
        }
    }
}
