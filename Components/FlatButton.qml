/**
 * @file FlatButton.qml
 * @brief Flat button component for CASE CONSTRUCTION UI.
 *
 * This QML file provides a scalable, stylable flat button with optional icon and arrow,
 * used for menu and list navigation.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0


Rectangle {
    id: flatBtn
    width: parent ? parent.width : 360
    height: rowItem.height + divider.height

    /**
     * @property title
     * @brief Button label text.
     */
    property string title: ""

    /**
     * @property iconSource
     * @brief URL for the button icon image.
     */
    property string iconSource: ""

    /**
     * @property isArrow
     * @brief Whether to show the arrow indicator on the right.
     */
    property bool isArrow: true

    /**
     * @signal clicked
     * @brief Emitted when the button is clicked.
     */
    signal clicked()

    /**
     * @property designWidth
     * @brief Reference width for UI scaling.
     */
    property real designWidth: 360

    /**
     * @property uiScale
     * @brief UI scale factor based on current width.
     */
    property real uiScale: {
        var s = flatBtn.width / designWidth;
        if (s < 0.6) s = 0.6;
        if (s > 1.2) s = 1.2;   // max 1.2x
        return s;
    }

    color: Styles.color.background

    Rectangle {
        id: rowItem
        width: parent.width
        height: Math.round(56 * flatBtn.uiScale)
        color: Styles.color.background

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: rowItem.color = Styles.color.darkSlateBlue
            onExited:  rowItem.color = Styles.color.background
            onPressed: rowItem.color = Styles.color.steelNavyBlue
            onReleased: {
                rowItem.color = Styles.color.darkSlateBlue
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
                color: Styles.color.lightGray
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                leftPadding: 8 * flatBtn.uiScale
            }

            Text {
                visible: flatBtn.isArrow
                text: Styles.text.arrow
                font.pixelSize: Math.round(20 * flatBtn.uiScale)
                color: Styles.color.pureWhite
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
        color: Styles.color.pureWhite
        opacity: 0.06
        anchors.bottom: parent.bottom
    }
}