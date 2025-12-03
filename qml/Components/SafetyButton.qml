/**
 * @file SafetyButton.qml
 * @brief Safety button component for CASE CONSTRUCTION UI.
 *
 * This QML file provides a reusable button with highlight and selection states, used for safety and navigation controls.
 */
import QtQuick
import QtQuick.Controls
import Styles 1.0
import "../Components"


Item {
    id: safetyButton

    /**
     * @property source
     * @brief URL for the button icon image (alias for iconImage.source).
     */
    property alias source: iconImage.source

    /**
     * @property selected
     * @brief Whether the button is currently selected.
     */
    property bool selected: false

    /**
     * @signal clicked
     * @brief Emitted when the button is clicked.
     */
    signal clicked()

    /**
     * @property showHighlightBar
     * @brief Whether to show the highlight bar at the bottom.
     */
    property bool showHighlightBar: true

    /**
     * @property buttonHighlight
     * @brief Whether to highlight the button background.
     */
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
