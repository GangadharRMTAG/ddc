/**
 * @file NavigationBar.qml
 * @brief Navigation bar for NextGen Display UI.
 *
 * This QML file provides a responsive navigation bar with menu, home, and control buttons,
 * supporting both portrait and landscape layouts. The EZEH button cycles through multiple
 * states, and the bar adapts its layout based on orientation.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
import QtQuick 2.15
import Styles 1.0
import ScreenUtils 1.0
import "../Components"

Item {
    id: navigationBar

    /**
     * @property isMenuSelected
     * @brief Indicates if the menu is selected (shows menu or home button).
     */
    property bool isMenuSelected: true

    /**
     * @property ezehSelected
     * @brief Index of the currently selected EZEH level.
     */
    property int ezehSelected: 0

    /**
     * @property ezehLevels
     * @brief Array of EZEH level icon URLs.
     */
    property var ezehLevels: [
        "qrc:/Images/NavigationBar/EZEHLow.svg",
        "qrc:/Images/NavigationBar/EZEHMid.svg",
        "qrc:/Images/NavigationBar/EZEHHigh.svg",
        "qrc:/Images/NavigationBar/EZEHCustom.svg"]


    Rectangle {
        id: navigationBarRect
        anchors.fill: parent
        color: Styles.color.pureBlack

        /**
         * @brief Loader for switching between portrait and landscape layouts.
         *
         * Loads the appropriate layout based on the isPortrait property.
         */
        Loader {
            anchors.fill: parent
            sourceComponent: isPortrait ? portraitLayout : landscapeLayout
        }
    }

    /**
     * @brief Portrait layout for the navigation bar.
     */
    Component {
        id: portraitLayout
        Rectangle {
            id: navigationBarRectP
            anchors.fill: parent
            color: Styles.color.pureBlack
            Rectangle{
                id: menuBtnRectP
                height: ScreenUtils.scaledWidth(navigationBar.width, 108)
                width: ScreenUtils.scaledWidth(navigationBar.width, 108)
                anchors{
                    top: parent.top;
                    left: parent.left;
                    topMargin: ScreenUtils.scaledHeight(mainWindow.height, 8);
                    leftMargin: ScreenUtils.scaledWidth(navigationBar.width, 14)
                }
                color: Styles.color.transparent
                Image {
                    id: menuBtnImgP
                    height: ScreenUtils.scaledWidth(navigationBar.width, 80)
                    width: ScreenUtils.scaledWidth(navigationBar.width, 80)
                    anchors.centerIn: parent
                    source: isMenuSelected ? "qrc:/Images/NavigationBar/BurgerMenu.svg" : "qrc:/Images/NavigationBar/BackBtn.svg"
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
                height: ScreenUtils.scaledWidth(navigationBar.width, 108)
                width: ScreenUtils.scaledWidth(navigationBar.width, 108)
                anchors{
                    right: parent.right;
                    top: parent.top;
                    topMargin: ScreenUtils.scaledHeight(mainWindow.height, 8);
                    rightMargin: ScreenUtils.scaledWidth(navigationBar.width, 14)
                }
                color: Styles.color.transparent
                visible: !isMenuSelected
                Image {
                    id: homeBtnImgP
                    height: ScreenUtils.scaledWidth(navigationBar.width, 70)
                    width: ScreenUtils.scaledWidth(navigationBar.width, 70)
                    anchors.centerIn: parent
                    source: "qrc:/Images/NavigationBar/Home.svg"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        isMenuSelected = !isMenuSelected
                    }
                }
            }
            Row{
                anchors{
                    verticalCenter: parent.verticalCenter;
                    right: parent.right;
                    rightMargin: ScreenUtils.scaledWidth(navigationBar.width, 14)
                }
                spacing: ScreenUtils.scaledWidth(navigationBar.width, 4)
                visible: isMenuSelected
                SafetyButton {
                    id: autoRideControlBtnP
                    height: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    source:  "qrc:/Images/NavigationBar/ARC.svg"
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.width, 55)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.width, 55)
                }
                SafetyButton {
                    id :autoIdleBtnP
                    height: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    source:  "qrc:/Images/NavigationBar/AutoIdle.svg"
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.width, 55)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.width, 55)
                }
                SafetyButton {
                    id: ezehBtnP
                    height: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.width, 108)
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.width, 80)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.width, 80)
                    showHighlightBar: false
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

    /**
     * @brief Landscape layout for the navigation bar.
     */
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
                    source: isMenuSelected ? "qrc:/Images/NavigationBar/BurgerMenu.svg" : "qrc:/Images/NavigationBar/BackBtn.svg"
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
                    source: "qrc:/Images/NavigationBar/Home.svg"
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
                SafetyButton {
                    id: autoRideControlBtnL
                    height: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    source:  "qrc:/Images/NavigationBar/ARC.svg"
                }
                SafetyButton {
                    id :autoIdleBtnL
                    height: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    source:  "qrc:/Images/NavigationBar/AutoIdle.svg"
                }
                SafetyButton {
                    id: ezehBtnL
                    height: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    width: ScreenUtils.scaledWidth(navigationBar.height, 108)
                    iconWidth: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    iconHeight: ScreenUtils.scaledWidth(navigationBar.height, 80)
                    showHighlightBar: false
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
