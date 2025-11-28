import QtQuick 2.15
import QtQuick.Layouts 1.15
import Styles 1.0
import "../Components"

Item {
    id: gaugesArea


    Rectangle {
        id: gaugesAreaRect
        anchors.fill: parent
        color: Styles.color.charcolNavy

        Loader {
            id: orientationLoader
            anchors.fill: parent
            active: true
            sourceComponent: isPortrait ? potraitLayout : landscapeLayout
        }
    }

    Component {
        id: potraitLayout
        Rectangle {
            color: Styles.color.transparent
            Column{
                anchors.fill: parent
                Rectangle{
                    height: parent.height * 0.25
                    width: parent.width
                    color: Styles.color.transparent
                    CurvedRectangle{
                        height: parent.height
                        width: parent.width * 0.35
                        anchors.left: parent.left
                        Text {
                            anchors.centerIn: parent
                            text: Qt.formatTime(new Date(), "hh:mm AP")
                            font{pixelSize: 20;bold: true}
                            color: Styles.color.textLight
                        }
                    }
                    Rectangle{
                        height: parent.height
                        width: parent.width * 0.3
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: Styles.color.background
                    }
                    CurvedRectangle{
                        height: parent.height
                        width: parent.width * 0.35
                        anchors.right: parent.right
                        color: Styles.color.accHeadTextSelected
                        Text {
                            anchors.centerIn: parent
                            text:"99999.9 H"
                            font{pixelSize: 20;bold: true}
                            color: Styles.color.textLight
                        }
                    }
                }
                Rectangle{
                    height: parent.height * 0.75
                    width: parent.width
                    color: Styles.color.transparent
                    Grid{
                        anchors.fill: parent
                        columns: 3
                        rows: 2
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/FuelLevel_R.png"
                            indicatorPos: 0
                            indicatorVal: 1
                        }
                        Rectangle{
                            height: parent.height/2
                            width: parent.width/3
                            color: Styles.color.background
                            Image {
                                height: parent.height * 0.7
                                width: parent.width * 0.7
                                anchors.centerIn: parent
                                source: "qrc:/Images/CASE_logo.png"
                            }
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/EngineCoolant_W.png"
                            indicatorPos: 1
                            indicatorVal: 3
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/DefLevel_W.png"
                            indicatorPos: 0
                            indicatorVal: 4
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/BatteryVoltage_W.png"
                            indicatorPos: 2
                            indicatorVal: 3
                        }
                        GaugeInfoBtn{
                            height: parent.height/2
                            width: parent.width/3
                            sourceImg: "qrc:/Images/GaugesArea/HydraulicOil_W.png"
                            indicatorPos: 1
                            indicatorVal: 3
                        }
                    }
                }
            }
        }

    }

    Component {
        id: landscapeLayout
        Rectangle {
            anchors.fill: parent
            color: Styles.color.background
            Rectangle {
                width: parent.width
                height: parent.height * 0.9
                anchors.verticalCenter: parent.verticalCenter
                color: Styles.color.transparent
                Column{
                    anchors.fill: parent
                    Rectangle{
                        height: parent.height/6
                        width: parent.width
                        color: Styles.color.background
                        Image {
                            height: parent.height * 0.7
                            width: parent.width * 0.7
                            anchors.centerIn: parent
                            source: "qrc:/Images/CASE_logo.png"
                        }
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/FuelLevel_R.png"
                        indicatorPos: 0
                        indicatorVal: 1
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/EngineCoolant_W.png"
                        indicatorPos: 1
                        indicatorVal: 3
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/DefLevel_W.png"
                        indicatorPos: 0
                        indicatorVal: 4
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/BatteryVoltage_W.png"
                        indicatorPos: 2
                        indicatorVal: 3
                    }
                    GaugeInfoBtn{
                        height: parent.height/6
                        width: parent.width
                        sourceImg: "qrc:/Images/GaugesArea/HydraulicOil_W.png"
                        indicatorPos: 1
                        indicatorVal: 3
                    }
                }
            }
        }
    }
}
