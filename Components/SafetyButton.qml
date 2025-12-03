import QtQuick
import QtQuick.Controls
import Styles 1.0
import "../Components"

Item {
    id: safetyButton

    property alias source: iconImage.source
    property bool selected: false
    signal clicked()
    property bool showHighlightBar: true
    property bool buttonHighlight: false

    Rectangle {
        id: tile
        anchors.fill: parent
        radius: 6
        color:buttonHighlight ? Styles.color.lavenderGray : Styles.color.charcolBlue
        Rectangle {
            height: showHighlightBar ? (tile.height - (progressBar.height * 2)) : tile.height
            width: tile.width
            color: Styles.color.transparent
            Image {
                id: iconImage
                width: tile.height*0.6
                height: tile.width*0.6
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: progressBar
            width: tile.width*0.8
            height: tile.height/10
            radius: height / 2
            color: safetyButton.selected ? Styles.color.lightBackground : Styles.color.pureBlack
            visible: showHighlightBar
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: tile.height/10
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: {
                safetyButton.selected = !safetyButton.selected
                safetyButton.clicked()
            }
        }
    }
}
