import QtQuick 2.12
import Styles 1.0
import QtQuick.Effects
import "../../Components"

Item {
    id: radio
    property int currRadio: 1
    property int priRadio: (currRadio === 0) ? radioChannels.count-1 : currRadio-1
    property int nextRadio: (currRadio === (radioChannels.count-1)) ? 0 : currRadio+1

    ListModel {
        id: radioChannels
        ListElement {name: "VIRGIN RADIO 104.5 FM"; program: "Morning Glory";      icon: "qrc:/Images/Radio/VirginRadio.png" }
        ListElement {name: "CNN RADIO 92.7 FM";     program: "The Daily DC";       icon: "qrc:/Images/Radio/CNN.png" }
        ListElement {name: "ESPN RADIO 91.1 FM";    program: "NBA commentary ";    icon: "qrc:/Images/Radio/NBAradio.jpg" }
    }


    Rectangle {
        id: mainRadioRect
        anchors.fill: parent
        color: Styles.color.transparent
        Column {
            anchors.centerIn: parent
            spacing: mainRadioRect.height * 0.05
            Rectangle {
                id: radioImages
                height: mainRadioRect.height * 0.45
                width: mainRadioRect.width * 0.65
                color: Styles.color.transparent
                Image {
                    id: leftChannel
                    height: radioImages.height/2
                    width: radioImages.width * 0.25
                    anchors {verticalCenter: parent.verticalCenter; right: currentChannel.left }
                    source: radioChannels.get(priRadio).icon
                }
                Image {
                    id: rightChannel
                    height: radioImages.height/2
                    width: radioImages.width * 0.25
                    anchors {verticalCenter: parent.verticalCenter; left: currentChannel.right }
                    source: radioChannels.get(nextRadio).icon
                }
                Image {
                    id: currentChannel
                    height: radioImages.height
                    width: radioImages.width * 0.5
                    anchors {horizontalCenter: parent.horizontalCenter}
                    source: radioChannels.get(currRadio).icon
                }
                MultiEffect {
                    anchors.fill: currentChannel
                    source: currentChannel
                    shadowEnabled: true
                    shadowColor:  Styles.color.pureBlack
                    shadowBlur: 1.0
                    shadowScale: 1.15
                    shadowVerticalOffset: 0
                    shadowHorizontalOffset: 0
                }
            }
            Rectangle {
                id: radioRect
                height: mainRadioRect.height * 0.12
                width: mainRadioRect.width * 0.65
                color: Styles.color.pureBlack
                radius: 5
                Text {
                    id: radioNameTXT
                    anchors.centerIn: parent
                    text: radioChannels.get(currRadio).name
                    font{pixelSize: 17;family: Styles.font.notoSans}
                    color: Styles.color.textLight
                }
                Text {
                    id: programNameTXT
                    anchors {horizontalCenter: parent.horizontalCenter; top: parent.bottom;topMargin: radioRect.height * 0.2}
                    text: radioChannels.get(currRadio).program
                    font{pixelSize: 17;family: Styles.font.notoSans}
                    color: Styles.color.textLight
                }
                Image {
                    id: leftArrow
                    height: radioRect.height
                    width: radioRect.height
                    anchors {verticalCenter: parent.verticalCenter; right: parent.left;rightMargin: radioRect.width * 0.1 }
                    rotation: 180
                    source: "qrc:/Images/Radio/Arrow.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(currRadio === 0)
                                currRadio = radioChannels.count - 1
                            else
                                currRadio--
                        }
                    }
                }
                Image {
                    id: rightArrow
                    height: radioRect.height
                    width: radioRect.height
                    anchors {verticalCenter: parent.verticalCenter; left: parent.right;leftMargin: radioRect.width * 0.1 }
                    source: "qrc:/Images/Radio/Arrow.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(currRadio === radioChannels.count - 1)
                                currRadio = 0
                            else
                                currRadio++
                        }
                    }
                }
            }
        }
    }
}
