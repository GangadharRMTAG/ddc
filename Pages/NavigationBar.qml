import QtQuick 2.15
import Styles 1.0
import "../Components"

Item {
    id: navigationBar
    property bool isMenuSelected: true
    property int ezehSelected: 0
    property var ezehLevels: [
        "qrc:/Images/NavigationBar/EZEHLow.png",
        "qrc:/Images/NavigationBar/EZEHMid.png",
        "qrc:/Images/NavigationBar/EZEHHigh.png",
        "qrc:/Images/NavigationBar/EZEHCustom.png"]

    Rectangle {
        id: navigationBarRect
        anchors.fill: parent
        color: Styles.color.pureBlack
        Rectangle{
            id: menuBtnRect
            height: parent.height * 0.8
            width: parent.height * 0.8
            anchors{top: parent.top;left: parent.left;topMargin: height*0.1;leftMargin: height*0.2}
            color: Styles.color.transparent
            Image {
                id: menuBtnImg
                height: parent.height * 0.8
                width: parent.width * 0.8
                anchors.centerIn: parent
                source: isMenuSelected ? "qrc:/Images/NavigationBar/BurgerMenu.png" : "qrc:/Images/NavigationBar/BackBtn.png"
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    isMenuSelected = !isMenuSelected
                }
            }
        }
        Row{
            anchors{verticalCenter: parent.verticalCenter; right: parent.right;rightMargin: navigationBarRect.height * 0.1}
            spacing: 3
            visible: isMenuSelected
            SafetyAreaBtn {
                id: autoRideControlBtn
                height: navigationBarRect.height * 0.8
                width: navigationBarRect.height * 0.8
                source:  "qrc:/Images/NavigationBar/ARC.png"
            }
            SafetyAreaBtn {
                id :autoIdleBtn
                height: navigationBarRect.height * 0.8
                width: navigationBarRect.height * 0.8
                source:  "qrc:/Images/NavigationBar/AutoIdle.png"
            }
            SafetyAreaBtn {
                id: ezehBtn
                height: navigationBarRect.height * 0.8
                width: navigationBarRect.height * 0.8
                showProgressBar: false
                source: ezehLevels[ezehSelected]
                onClicked: {
                    if(ezehSelected === 3)
                        ezehSelected = 0
                    else
                        ezehSelected++
                }
            }
        }
    }
}
