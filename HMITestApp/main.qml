import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 420
    height: 720
    visible: true
    title: "Test CAN Publisher"

    Rectangle {
        anchors.fill: parent
        color: "#1E1E1E"

        Flickable {
            anchors.fill: parent
            contentWidth: width
            contentHeight: contentColumn.implicitHeight + 50
            clip: true
            flickableDirection: Flickable.VerticalFlick
            interactive: true
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }

            ColumnLayout {
                id: contentColumn
                width: parent.width + 20
                anchors.margins: 30
                spacing: 20

                //  RPM Widget
                Label {
                    text: "RPM"
                    color: "white"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                Slider {
                    id: rpmSlider
                    from: 0
                    to: 4000
                    stepSize: 1
                    value: 0
                    Layout.alignment: Qt.AlignHCenter

                    onMoved: {
                        zmqPublisher.publishRPM(Math.round(value))
                    }
                }

                Label {
                    text: Math.round(rpmSlider.value) + " x1000 RPM"
                    color: "#CCCCCC"
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }

                //  Top Bar Warning
                Label {
                    text: "Telltales"
                    color: "white"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                GridLayout {
                    columns: 2
                    columnSpacing: 20
                    rowSpacing: 12
                    Layout.alignment: Qt.AlignHCenter

                    // Manually defined checkboxes with names
                    CheckBox { text: "Stop";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(0, checked) }
                    CheckBox { text: "Caution"; checked: true;onCheckedChanged: zmqPublisher.publishTelltale(1, checked) }
                    CheckBox { text: "Seat Belt";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(2, checked) }
                    CheckBox { text: "Park Brake";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(3, checked) }
                    CheckBox { text: "Work Lamp";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(4, checked) }
                    CheckBox { text: "Beacon";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(5, checked) }
                    CheckBox { text: "Regeneration";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(6, checked) }
                    CheckBox { text: "Grid Heater";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(7, checked) }
                    CheckBox { text: "Hydraulic Lock";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(8, checked) }
                    CheckBox { text: "Foot Pedal";checked: true; onCheckedChanged: zmqPublisher.publishTelltale(9, checked) }
                }


                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }

                // Gauge Area

                Label {
                    text: "Gauges"
                    color: "white"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                GridLayout {
                    columns: 2
                    columnSpacing: 20
                    rowSpacing: 16
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Column { Label { text: "Fuel Level"; color: "white" }
                        Slider { from: 0; to: 100; value:12.5; stepSize: 12.5;
                            onValueChanged: zmqPublisher.publishGauge(0, value) } }

                    Column { Label { text: "Coolant Level"; color: "white" }
                        Slider { from: 0; to: 100; stepSize: 25;
                            onValueChanged: zmqPublisher.publishGauge(1, value) } }

                    Column { Label { text: "DEF Level"; color: "white" }
                        Slider { from: 0; to: 100; stepSize: 25;
                            onValueChanged: zmqPublisher.publishGauge(2, value) } }

                    Column { Label { text: "Battery Level"; color: "white" }
                        Slider { from: 0; to: 100; stepSize: 25;
                            onValueChanged: zmqPublisher.publishGauge(3, value) } }

                    Column { Label { text: "Hydraulic Level"; color: "white" }
                        Slider { from: 0; to: 100; stepSize: 25;
                            onValueChanged: zmqPublisher.publishGauge(4, value) } }
                    Column {

                        spacing: 6

                        Label {

                            text: "EH : " + engineHoursSlider.value.toFixed(1)

                            color: "white"

                        }

                        Slider {

                            id: engineHoursSlider

                            from: 0

                            to: 99999.9

                            stepSize: 0.1

                            property real lastValue: 0

                            Component.onCompleted: {

                                value = zmqPublisher.currentEngineHours()

                                lastValue = value

                                zmqPublisher.publishEngineHours(value)

                            }

                            onMoved: {

                                if (value < lastValue) {

                                    value = lastValue

                                    return

                                }

                                lastValue = value

                                zmqPublisher.publishEngineHours(value)

                            }

                        }

                        Button {

                            text: "Reset EH"

                            width: 120

                            height: 32

                            onClicked: {

                                 zmqPublisher.publishEngineHours(0.0)
                                zmqPublisher.resetEngineHours()
                                engineHoursSlider.value = 0
                                engineHoursSlider.lastValue = 0

                            }

                        }

                    }




                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }

                // MAIN → TEST
                Label {
                    text: "Main App Commands"
                    color: "white"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 16

                    // ISO Button
                    Rectangle {
                        id: isoRect
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 80

                        radius: 8
                        color: zmqSubscriber.isoActive ? "#2ECC71" : "#555"

                        border.color: "red"
                        border.width: 5

                        Text {
                            anchors.centerIn: parent
                            text: zmqSubscriber.isoActive ? "H" : "ISO"
                            color: zmqSubscriber.isoActive ? "green" : "yellow"
                            font.pixelSize: 18
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                // Request toggle, but do NOT change UI locally
                                zmqPublisher.publishSaftyButtonStatus(
                                            0,
                                            !zmqSubscriber.isoActive
                                            )
                            }
                        }

                    }


                    // 🔹 Another button example
                    Rectangle {
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 80

                        radius: 8
                        color: "#555"
                        border.color: "blue"
                        border.width: 5

                        Text {
                            anchors.centerIn: parent
                            text: "BTN2"
                            color: "white"
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }

                    // 🔹 Creep button
                    Rectangle {
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 80

                        radius: 8
                        color: zmqSubscriber.creepActive? "#2ECC71" : "#555"
                        border.color: "orange"
                        border.width: 5

                        Text {
                            anchors.centerIn: parent

                            text: "Creep"
                            color: zmqSubscriber.creepActive ? "green" : "yellow"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                zmqPublisher.publishSaftyButtonStatus(
                                            2,                         // ✅ CREEP index
                                            !zmqSubscriber.creepActive
                                            )
                            }
                        }

                    }
                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }
                ComboBox {
                    id: comboBox
                    width: 220

                    // key = text (string), value = number
                    model: [
                        { key: "Info Popup", value: 10 }, // as per Sys2 doc 6.4.2 Critical
                        { key: "Soft Action Popup",   value: 20 },
                        { key: "Warning Popup",  value: 30 },
                        { key: "Critical Info Popup",  value: 40 },
                        { key: "Critical Action Popup",  value: 50 },
                        { key: "Info Popup1",  value: 60 },
                        { key: "Info Popup2",  value: 70 }
                    ]

                    textRole: "key"

                    onActivated: {
                        zmqPublisher.messagePopup(  model[currentIndex].value)
                        console.log("value: ",   model[currentIndex].value)
                    }
                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }

                Label {
                    text: "Trip Gauge Signals"
                    color: "white"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                GridLayout {
                    columns: 2
                    columnSpacing: 20
                    rowSpacing: 16
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    Column { Label { text: "Fuel Rate"; color: "white" }
                        Slider { from: 0; to: 100.0; value: 0; stepSize: 0.1;
                            onValueChanged: zmqPublisher.publishFuelRate(value) }}

                    Column { Label { text: "DEF Rate"; color: "white" }
                        Slider { from: 0; to: 100.0; stepSize: 0.1;
                            onValueChanged: zmqPublisher.publishDefRate(value) } }

                    Column { Label { text: "Engine Load"; color: "white" }
                        Slider { from: 0; to: 100; stepSize: 5;
                            onValueChanged: zmqPublisher.publishAvgEngineLoad(value) } }


                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#444"
                }


                Connections {
                    target: zmqSubscriber

                    function onIsoButtonChanged(pressed) {
                        console.log("ISO button from Main:", pressed)
                    }
                    function onCreepButtonChanged(pressed) {
                        console.log("Creep button from Main:", pressed)
                    }

                    function onFrameReceived(id, payload) {
                        console.log("Frame received from Main:", id.toString(16))
                    }
                }

                Item { Layout.fillHeight: true }
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "#444"
            }
            Item { Layout.fillHeight: true }

        }
    }
}
