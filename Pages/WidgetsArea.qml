import QtQuick 2.12
import QtQuick.Shapes 1.12
import Styles 1.0
import "../Components"

Rectangle {
    id: widgetArea
    implicitWidth: 420
    implicitHeight: 420
    color: Styles.color.transparent

    property real outerRadius:      width * 0.30
    property real darkRingRadius:   outerRadius * 0.92
    property real thinGreyRadius:   outerRadius * 0.89
    property real bigWhiteRadius:   outerRadius * 0.66
    property real smallWhiteRadius: outerRadius * 0.45

    property real numberRadius: thinGreyRadius + outerRadius * 0.20
    property real tickRadius:   thinGreyRadius + outerRadius * 0.05

    property int value0to100: 35
    property int rpmValue: 1250
    property url source: ""
    property int selectedIndex: 0

    function scaleAngle(v) {
        return 180 - (v / 40.0) * 180.0;
    }

    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/WidgetArea/SpeedMeater.png" }
        ListElement { icon: "qrc:/Images/WidgetArea/TripInfo.png" }
        ListElement { icon: "qrc:/Images/WidgetArea/CameraView.png" }
        ListElement { icon: "qrc:/Images/WidgetArea/Radio.png" }
        ListElement { icon: "qrc:/Images/WidgetArea/Climate.png" }
        ListElement { icon: "qrc:/Images/WidgetArea/MachineInfo.png" }
    }

    Row {
        id: buttonsRow
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Repeater {
            model: buttonModel
            delegate: SafetyButton {
                height: isPortrait ? widgetArea.height * 0.18 : widgetArea.height * 0.16
                width:  isPortrait ? widgetArea.width  * 0.18 : widgetArea.width  * 0.16
                showHighlightBar: false
                buttonHighlight : index === selectedIndex
                source: icon
                onClicked: {
                    selectedIndex = index
                }
            }
        }
    }

    Rectangle {
        id: widgetBackground
        x: 0
        y: buttonsRow.height
        width: isPortrait ? parent.width * 1.05 : parent.width
        height: parent.height - buttonsRow.height
        anchors.horizontalCenter: parent.horizontalCenter
        color: Styles.color.darkSlate
    }

    Loader {
        id: gaugeLoader
        anchors.fill: widgetBackground
        sourceComponent: gaugeComponent
    }

    Component {
        id: gaugeComponent

        Item {
            id: gauge
            anchors.fill: parent

            property real cx: width / 2
            property real cy: height / 2

            property real outerRadius:      widgetArea.outerRadius
            property real darkRingRadius:   widgetArea.darkRingRadius
            property real thinGreyRadius:   widgetArea.thinGreyRadius
            property real bigWhiteRadius:   widgetArea.bigWhiteRadius
            property real smallWhiteRadius: widgetArea.smallWhiteRadius
            property real numberRadius:     widgetArea.numberRadius
            property real tickRadius:       widgetArea.tickRadius

            function scaleAngle(v) {
                return widgetArea.scaleAngle(v)
            }

            function xOnCircle(angleDeg, r) {
                return cx + r * Math.cos(angleDeg * Math.PI / 180)
            }
            function yOnCircle(angleDeg, r) {
                return cy - r * Math.sin(angleDeg * Math.PI / 180)
            }

            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.smooth: true
                antialiasing: true
                layer.samples: 8

                ShapePath {
                    strokeWidth: outerRadius * 0.09
                    strokeColor: Styles.color.midnightBlue
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.RoundCap

                    startX: cx + darkRingRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: darkRingRadius
                        radiusY: darkRingRadius
                        startAngle: 0
                        sweepAngle: 359.9
                    }
                }

                ShapePath {
                    strokeWidth: outerRadius * 0.02
                    strokeColor: Styles.color.slateBlue
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.RoundCap

                    startX: cx + thinGreyRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: thinGreyRadius
                        radiusY: thinGreyRadius
                        startAngle: 0
                        sweepAngle: 359.9
                    }
                }

                ShapePath {
                    strokeWidth: outerRadius * 0.4
                    strokeColor: Styles.color.mistBlue
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.RoundCap

                    startX: cx + bigWhiteRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: bigWhiteRadius
                        radiusY: bigWhiteRadius
                        startAngle: 0
                        sweepAngle: 359.9
                    }
                }
            }

            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.smooth: true
                antialiasing: true
                layer.samples: 8

                ShapePath {
                    strokeWidth: outerRadius * 0.4
                    strokeColor: Styles.color.lightBackground
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.FlatCap

                    startX: cx - bigWhiteRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: bigWhiteRadius
                        radiusY: bigWhiteRadius
                        startAngle: 180
                        sweepAngle: widgetArea.value0to100 * 1.8
                    }
                }

                ShapePath {
                    strokeWidth: outerRadius * 0.05
                    strokeColor: Styles.color.nightIndigo
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.RoundCap

                    startX: cx + smallWhiteRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: smallWhiteRadius
                        radiusY: smallWhiteRadius
                        startAngle: 0
                        sweepAngle: 359.9
                    }
                }

                ShapePath {
                    strokeWidth: outerRadius * 0.05
                    strokeColor: Styles.color.errorColor
                    fillColor: Styles.color.transparent
                    capStyle: ShapePath.FlatCap

                    startX: cx - smallWhiteRadius
                    startY: cy

                    PathAngleArc {
                        centerX: cx
                        centerY: cy
                        radiusX: smallWhiteRadius
                        radiusY: smallWhiteRadius
                        startAngle: 180
                        sweepAngle: widgetArea.value0to100 * 1.8
                    }
                }
            }

            Repeater {
                model: [0, 10, 20, 30, 40]
                delegate: Text {
                    property real a: gauge.scaleAngle(modelData)
                    x: gauge.xOnCircle(a, gauge.numberRadius) - width / 2
                    y: gauge.yOnCircle(a, gauge.numberRadius) - height / 2

                    text: modelData
                    color: Styles.color.lightBackground
                    font.pixelSize: gauge.width * 0.04
                }
            }

            Repeater {
                model: [10, 20, 30, 40]
                delegate: Rectangle {
                    property real a: gauge.scaleAngle(modelData)
                    width: 2
                    height: 5
                    radius: 1
                    color: Styles.color.lightBackground

                    x: gauge.xOnCircle(a, gauge.tickRadius) - width / 2
                    y: gauge.yOnCircle(a, gauge.tickRadius) - height / 2
                    transformOrigin: Item.Center
                    rotation: -a + 90
                }
            }

            Rectangle {
                width: 2
                height: 5
                radius: 1
                color: Styles.color.lightBackground

                property real a: gauge.scaleAngle(20)
                x: gauge.xOnCircle(a, gauge.tickRadius) - width / 2
                y: gauge.yOnCircle(a, gauge.tickRadius) - height / 2
                transformOrigin: Item.Center
                rotation: -a + 90
            }

            Rectangle {
                width: 5
                height: 2
                radius: 1
                color: Styles.color.lightBackground

                property real a: gauge.scaleAngle(0)
                x: gauge.xOnCircle(a, gauge.tickRadius)
                y: gauge.yOnCircle(a, gauge.tickRadius) - height / 2
            }

            Rectangle {
                id: centerCircle
                width: outerRadius * 0.85
                height: width
                radius: width / 2
                x: cx - width / 2
                y: cy - height / 2
                color: Styles.color.midnightBlue
            }

            Column {
                anchors.horizontalCenter: centerCircle.horizontalCenter
                anchors.verticalCenter: centerCircle.verticalCenter
                spacing: centerCircle.height * 0.05

                Text {
                    text: widgetArea.rpmValue
                    color: Styles.color.lightBackground
                    font.pixelSize: centerCircle.height * 0.30
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "RPM"
                    color: Styles.color.mistLavender
                    font.pixelSize: centerCircle.height * 0.11
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: centerCircle.height * 0.10

                    Rectangle {
                        id: imgRect
                        height: width * 0.9
                        width: centerCircle.width * 0.12
                        color: Styles.color.transparent

                        Image {
                            id: iconImage
                            width: imgRect.width * 2
                            height: imgRect.height * 2
                            source: widgetArea.source
                        }
                    }

                    Text {
                        text: "n/min"
                        color: Styles.color.lightBackground
                        font.pixelSize: centerCircle.height * 0.09
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
}
