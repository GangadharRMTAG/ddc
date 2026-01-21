import QtQuick 2.12
import QtQuick.Shapes 1.12
import Styles 1.0

Item {
    id: gauge
    anchors.fill: parent
    anchors.topMargin: parent.height * 0.08
    /* ===================== Properties ===================== */

    property int rpmValue: appInterface.rpm

    property real value0to100: Math.max(0,Math.min(40,rpmValue / 100.0))

    property url source: "qrc:/Images/WidgetArea/Idel.png"

    /* ===================== Geometry ===================== */

    property real cx: width / 2
    property real cy: height / 2

    property real outerRadius: width * 0.33

    property real outerBlueRadius: outerRadius * 0.90
    property real outerWhiteRadius: outerRadius * 0.90

    property real innerCircleRadius: outerRadius * 0.58
    property real innerGreyRadius: innerCircleRadius * 1.05

    /* 🔧 IMPORTANT FIX: move white arc outward */
    property real innerWhiteRingRadius: innerCircleRadius * 1.02

    property real numberRadius: outerRadius * 1.15
    property real dashRadius: outerRadius * 1.02

    /* ===================== Helpers ===================== */

    function scaleAngle(v) {
        return 180 - (v / 40.0) * 180.0
    }

    function xOnCircle(a, r) {
        return cx + r * Math.cos(a * Math.PI / 180)
    }

    function yOnCircle(a, r) {
        return cy - r * Math.sin(a * Math.PI / 180)
    }

    /* ===================== Outer Rings ===================== */

    Shape {
        anchors.fill: parent
        antialiasing: true
        layer.enabled: true
        layer.samples: 8

        /* Thick blueish ring */
        ShapePath {
            strokeWidth: outerRadius * 0.12
            strokeColor: Styles.color.midnightBlue
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            startX: cx + outerBlueRadius
            startY: cy

            PathAngleArc {
                centerX: cx
                centerY: cy
                radiusX: outerBlueRadius
                radiusY: outerBlueRadius
                startAngle: 0
                sweepAngle: 359.9
            }
        }

        /* Thin white RPM progress */
        ShapePath {
            strokeWidth: outerRadius * 0.02
            strokeColor: Styles.color.lightBackground
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            startX: cx - outerWhiteRadius
            startY: cy

            PathAngleArc {
                centerX: cx
                centerY: cy
                radiusX: outerWhiteRadius
                radiusY: outerWhiteRadius
                startAngle: 180
                sweepAngle: (value0to100 / 40.0) * 180.0
            }
        }
    }

    /* ===================== Scale Dashes ===================== */

    Repeater {
        model: [0, 10, 20, 30, 40]

        delegate: Rectangle {
            width: 1.5
            height: 6
            radius: 1
            color: Styles.color.lightBackground

            property real a: gauge.scaleAngle(modelData)

            x: gauge.xOnCircle(a, dashRadius) - width / 2
            y: gauge.yOnCircle(a, dashRadius) - height / 2

            transformOrigin: Item.Center
            rotation: -a + 90
        }
    }

    /* ===================== Scale Numbers ===================== */

    Repeater {
        model: [0, 10, 20, 30, 40]

        delegate: Text {
            property real a: gauge.scaleAngle(modelData)

            x: gauge.xOnCircle(a, numberRadius) - width / 2
            y: gauge.yOnCircle(a, numberRadius) - height / 2

            text: modelData
            color: Styles.color.lightBackground
            font.pixelSize: gauge.width * 0.045
        }
    }

    /* ===================== Inner Circles ===================== */

    Shape {
        anchors.fill: parent
        antialiasing: true
        layer.enabled: true
        layer.samples: 8

        ShapePath {
            strokeWidth: innerCircleRadius * 0.12
            strokeColor: "grey"
            fillColor: "transparent"
            capStyle: ShapePath.FlatCap

            startX: cx + innerWhiteRingRadius
            startY: cy

            PathAngleArc {
                centerX: cx
                centerY: cy
                radiusX: innerWhiteRingRadius
                radiusY: innerWhiteRingRadius
                startAngle: 0
                sweepAngle: 359.9
            }
        }
    }

    /* White semicircular ring on grey border */
    Shape {
        anchors.fill: parent
        antialiasing: true
        layer.enabled: true
        layer.samples: 8

        ShapePath {
            strokeWidth: innerCircleRadius * 0.08
            strokeColor: "white"
            fillColor: "transparent"
            capStyle: ShapePath.FlatCap

            startX: cx - innerWhiteRingRadius
            startY: cy

            PathAngleArc {
                centerX: cx
                centerY: cy
                radiusX: innerWhiteRingRadius
                radiusY: innerWhiteRingRadius
                startAngle: 180
                sweepAngle: (value0to100 / 40.0) * 180.0
            }
        }
    }

    /* Center fill */
    Rectangle {
        id: centerCircle
        width: innerCircleRadius * 2
        height: width
        radius: width / 2
        x: cx - width / 2
        y: cy - height / 2
        color: Styles.color.midnightBlue
    }

    /* ===================== Center Content ===================== */

    Column {
        anchors.centerIn: centerCircle
        spacing: centerCircle.height * 0.05



        Text {
            text: rpmValue
            color: Styles.color.lightBackground
            font.pixelSize: centerCircle.height * 0.20
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            transform: Translate {
                y: centerCircle.height * 0.09
            }
        }

        Text {
            text: "RPM"
            color: Styles.color.mistLavender
            font.pixelSize: centerCircle.height * 0.10
            anchors.horizontalCenter: parent.horizontalCenter
            transform: Translate {
                y: centerCircle.height * 0.03
            }
        }

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: centerCircle.height * 0.10

            Rectangle {
                id: imgRect
                height: width * 0.9
                width: centerCircle.width * 0.099
                color: Styles.color.transparent

                Image {
                    id: iconImage
                    width: imgRect.width * 2
                    height: imgRect.height * 2
                    source: gauge.source
                }
            }

            Text {
                text: "n/min"
                color: Styles.color.lightBackground
                font.pixelSize: centerCircle.height * 0.07
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter


            }
        }
    }
}
