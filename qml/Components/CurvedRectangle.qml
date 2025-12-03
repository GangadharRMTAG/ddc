/**
 * @file CurvedRectangle.qml
 * @brief Curved rectangle background component for CASE CONSTRUCTION UI.
 *
 * This QML file provides a reusable rectangle with a background image, used for styled UI sections.
 */
import QtQuick
import Styles 1.0

Rectangle{
    id:curvedRectangle
    color: Styles.color.background

    /**
     * @brief Background image filling the rectangle.
     */
    Image {
        id: backgroundimg
        anchors.fill: parent
        source: "qrc:/Images/GaugesArea/Background.png"
    }
}
