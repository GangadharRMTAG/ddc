/**
 * @file CurvedRectangle.qml
 * @brief Curved rectangle background component for NextGen Display UI.
 *
 * This QML file provides a reusable component that draws a curved rectangle shape. It uses QtQuick Shapes
 * to create a scalable curved rectangle that can be customized in size and color. The shape is defined using
 * cubic Bezier curves to achieve the desired curvature.
 *
 * @date 16-Dec-2025
 * @author Gangadhar Thalange
 * @last modified -  Malhar Sapatnekar
 */
import QtQuick 2.15
import QtQuick.Shapes 1.15

Item {
    id: curvedRect


    // Reference design size as per figma
    property real refWidth: 272
    property real refHeight: 45

    // Scale factors
    readonly property real sx: width / refWidth
    readonly property real sy: height / refHeight

    // Drawn based on cubic Bezier curve.
    readonly property real topLeftX: 0
    readonly property real topLeftY: 0

    readonly property real topRightX: refWidth * sx
    readonly property real topRightY: 0

    readonly property real middleRightX: 240 * sx
    readonly property real middleRightY: 35 * sy

    readonly property real bottomRightX: 217 * sx
    readonly property real bottomRightY: refHeight * sy

    readonly property real bottomLeftX: 55 * sx
    readonly property real bottomLeftY: refHeight * sy

    readonly property real middleLeftX: 32 * sx
    readonly property real middleLeftY: 35 * sy

    readonly property real rightCurveP1X: 232 * sx
    readonly property real rightCurveP1Y: refHeight * sy

    readonly property real rightCurveP2X: 224 * sx
    readonly property real rightCurveP2Y: refHeight * sy

    readonly property real leftCurveP1X: 48 * sx
    readonly property real leftCurveP1Y: refHeight * sy

    readonly property real leftCurveP2X: 40 * sx
    readonly property real leftCurveP2Y: refHeight * sy

    property color rectColor: "#000000"

    layer.enabled: true
    layer.samples: 4

    Shape {
        anchors.fill: parent

        ShapePath {
            fillColor: curvedRect.rectColor
            strokeWidth: 0

            startX: topLeftX
            startY: topLeftY

            PathLine { x: topRightX; y: topRightY } //top edge
            PathLine { x: middleRightX; y: middleRightY } //right edge

            PathCubic { //right curve
                x: bottomRightX
                y: bottomRightY
                control1X: rightCurveP1X
                control1Y: rightCurveP1Y
                control2X: rightCurveP2X
                control2Y: rightCurveP2Y
            }

            PathLine { x: bottomLeftX; y: bottomLeftY } //bottom edge

            PathCubic { //left curve
                x: middleLeftX
                y: middleLeftY
                control1X: leftCurveP1X
                control1Y: leftCurveP1Y
                control2X: leftCurveP2X
                control2Y: leftCurveP2Y
            }

            PathLine { x: topLeftX; y: topLeftY } //right edge
        }
    }
}
