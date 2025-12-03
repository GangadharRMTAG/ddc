import QtQuick 2.12
import Styles 1.0
import "../../Components"

Item {
    id: climate

    property int fanSpeedLevel: 1 //1-low,2-mid,3-high
    property int cabinTemp: 20

    Rectangle {
        anchors.fill: parent
        color: Styles.color.transparent
        Rectangle {
            id: climateRect
            height: parent.height * 0.7
            width: parent.width * 0.7
            color: Styles.color.darkSlate
            anchors.centerIn: parent
            Row {
                anchors {top: parent.top; horizontalCenter: parent.horizontalCenter}
                spacing: climateRect.width/8
                SafetyButton {
                    id: defrostBtn
                    height: climateRect.height/3
                    width: climateRect.width * 0.25
                    source: "qrc:/Images/Climate/Defrost.png"
                    Text {
                        anchors {horizontalCenter: parent.horizontalCenter; bottom: parent.top;topMargin: parent.height/10}
                        text: "Defrost"
                        font{pixelSize: 17;family: Styles.font.notoSans}
                        color: Styles.color.textLight
                    }
                }
                SafetyButton {
                    id: acBtn
                    height: climateRect.height/3
                    width: climateRect.width * 0.25
                    source: "qrc:/Images/Climate/AirConditioning.png"
                    Text {
                        anchors {horizontalCenter: parent.horizontalCenter; bottom: parent.top;topMargin: parent.height/10}
                        text: "A/C"
                        font{pixelSize: 17;family: Styles.font.notoSans}
                        color: Styles.color.textLight
                    }
                }
                SafetyButton {
                    id: recirculationBtn
                    height: climateRect.height/3
                    width: climateRect.width * 0.25
                    source: "qrc:/Images/Climate/Recirculation.png"
                    Text {
                        anchors {horizontalCenter: parent.horizontalCenter; bottom: parent.top;topMargin: parent.height/10}
                        text: "Recirc"
                        font{pixelSize: 17;family: Styles.font.notoSans}
                        color: Styles.color.textLight
                    }
                }
            }
            Row {
                anchors {bottom: parent.bottom; horizontalCenter: parent.horizontalCenter}
                spacing: climateRect.width * 0.1
                Rectangle{
                    id: cabinTemperature
                    height: climateRect.height/3
                    width: climateRect.width * 0.45
                    border.color: Styles.color.textDim
                    radius: 5
                    color: Styles.color.transparent
                    Row{
                        anchors.centerIn: parent
                        spacing: cabinTemperature.width/12
                        Image {
                            height: cabinTemperature.height/2
                            width: cabinTemperature.width/3
                            source: "qrc:/Images/Climate/CabinTemp.png"
                        }
                        Text {
                            text: cabinTemp + " °C"
                            font{pixelSize: 17;family: Styles.font.notoSans}
                            color: Styles.color.textLight
                        }
                    }
                }
                Rectangle{
                    id:  fanSpeed
                    height: climateRect.height/3
                    width: climateRect.width * 0.45
                    border.color: Styles.color.textDim
                    radius: 5
                    color: Styles.color.transparent
                    Row{
                        anchors.centerIn: parent
                        spacing: cabinTemperature.width/12
                        Image {
                            height: cabinTemperature.height/2
                            width: cabinTemperature.width/3
                            source: "qrc:/Images/Climate/Fan.png"
                        }
                        Rectangle{
                            id: fanspeedIndicator
                            height: fanSpeed.height * 0.4
                            width: fanSpeed.width * 0.3
                            color: Styles.color.transparent
                            Row{
                                anchors.fill: parent
                                spacing: 2
                                Rectangle{
                                    width: fanspeedIndicator.width/3
                                    height: fanspeedIndicator.height * 0.6
                                    color: fanSpeedLevel >= 1 ? Styles.color.textDim  : Styles.color.charcolBlue
                                    anchors {bottom: parent.bottom}
                                }
                                Rectangle{
                                    width: fanspeedIndicator.width/3
                                    height: fanspeedIndicator.height * 0.8
                                    color: fanSpeedLevel >= 2 ? Styles.color.textDim  : Styles.color.charcolBlue
                                    anchors {bottom: parent.bottom}
                                }
                                Rectangle{
                                    width: fanspeedIndicator.width/3
                                    height: fanspeedIndicator.height
                                    color: fanSpeedLevel >= 3 ? Styles.color.textDim  : Styles.color.charcolBlue
                                    anchors {bottom: parent.bottom}
                                }
                            }
                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                        if(fanSpeedLevel === 3)
                            fanSpeedLevel=0
                        else
                            fanSpeedLevel++
                        }
                    }
                }
            }
        }
    }
}
