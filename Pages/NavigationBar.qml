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
        Loader {
            anchors.fill: parent
            sourceComponent: isPortrait ? potraitLayout : landscapeLayout
        }
    }
    Component {
        id: potraitLayout
        Rectangle {
            id: navigationBarRectP
            anchors.fill: parent
            color: Styles.color.pureBlack
            Rectangle{
                id: menuBtnRectP
                height: parent.height * 0.8
                width: parent.height * 0.8
                anchors{top: parent.top;left: parent.left;topMargin: height*0.1;leftMargin: height*0.2}
                color: Styles.color.transparent
                Image {
                    id: menuBtnImgP
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
            Rectangle{
                id: homeBtnP
                height: parent.height * 0.8
                width: parent.height * 0.8
                anchors{right: parent.right;top: parent.top;rightMargin: height*0.2;topMargin: height*0.1}
                color: Styles.color.transparent
                visible: !isMenuSelected
                Image {
                    id: homeBtnImgP
                    height: parent.height * 0.8
                    width: parent.width * 0.8
                    anchors.centerIn: parent
                    source: "qrc:/Images/NavigationBar/Home.png"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        isMenuSelected = !isMenuSelected
                    }
                }
            }
            Row{
                anchors{verticalCenter: parent.verticalCenter; right: parent.right;rightMargin: navigationBarRectP.height * 0.1}
                spacing: 3
                visible: isMenuSelected
                SafetyAreaBtn {
                    id: autoRideControlBtnP
                    height: navigationBarRectP.height * 0.8
                    width: navigationBarRectP.height * 0.8
                    source:  "qrc:/Images/NavigationBar/ARC.png"
                }
                SafetyAreaBtn {
                    id :autoIdleBtnP
                    height: navigationBarRectP.height * 0.8
                    width: navigationBarRectP.height * 0.8
                    source:  "qrc:/Images/NavigationBar/AutoIdle.png"
                }
                SafetyAreaBtn {
                    id: ezehBtnP
                    height: navigationBarRectP.height * 0.8
                    width: navigationBarRectP.height * 0.8
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
    Component {
        id: landscapeLayout
        Rectangle {
            id: navigationBarRectL
            anchors.fill: parent
            color: Styles.color.pureBlack
            Rectangle{
                id: menuBtnRectL
                height: parent.width * 0.8
                width: parent.width * 0.8
                anchors{top: parent.top;left: parent.left;topMargin: height*0.2;leftMargin: height*0.1}
                color: Styles.color.transparent
                Image {
                    id: menuBtnImgL
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
            Rectangle{
                id: homeBtnL
                height: parent.width * 0.8
                width: parent.width * 0.8
                anchors{bottom: parent.bottom;left: parent.left;bottomMargin: height*0.2;leftMargin: height*0.1}
                color: Styles.color.transparent
                visible: !isMenuSelected
                Image {
                    id: homeBtnImgL
                    height: parent.height * 0.8
                    width: parent.width * 0.8
                    anchors.centerIn: parent
                    source: "qrc:/Images/NavigationBar/Home.png"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        isMenuSelected = !isMenuSelected
                    }
                }
            }
            Column{
                anchors{horizontalCenter: parent.horizontalCenter; bottom: parent.bottom;bottomMargin: navigationBarRectL.width * 0.1}
                spacing: 3
                visible: isMenuSelected
                SafetyAreaBtn {
                    id: autoRideControlBtnL
                    height: navigationBarRectL.width * 0.8
                    width: navigationBarRectL.width * 0.8
                    source:  "qrc:/Images/NavigationBar/ARC.png"
                }
                SafetyAreaBtn {
                    id :autoIdleBtnL
                    height: navigationBarRectL.width * 0.8
                    width: navigationBarRectL.width * 0.8
                    source:  "qrc:/Images/NavigationBar/AutoIdle.png"
                }
                SafetyAreaBtn {
                    id: ezehBtnL
                    height: navigationBarRectL.width * 0.8
                    width: navigationBarRectL.width * 0.8
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
}
