/**
* @file MainApp_P.qml
* @brief Portrait main application UI for NextGen Display.
*
* This QML file defines the main layout for portrait orientation, organizing the
* top bar, safety area, navigation bar, and main content area. It dynamically loads
* the home screen or settings screen based on the application state.
*
* @date 08-Dec-2025
* @author Gangadhar Thalange
*/
import QtQuick 2.12
import Styles 1.0
import ScreenUtils 1.0
import "./Pages"
import "./Components"

Item {
    id: portraitMain
    anchors.fill: parent

    /**
     * @brief Main vertical layout column.
     *
     * Contains the top bar, safety area, main content loader, and navigation bar.
     */
    Column {
        anchors.fill: parent
        TopBar {
            id: topBarP
            anchors.horizontalCenter: parent.horizontalCenter
            height: ScreenUtils.scaledHeight(portraitMain.height, 90)
            width: ScreenUtils.scaledWidth(portraitMain.width)
        }
        SafetyArea{
            id: safetyAreaP
            height:  ScreenUtils.scaledHeight(portraitMain.height, 140)
            width:   ScreenUtils.scaledWidth(portraitMain.width)
        }
        /**
         * @brief Loader for main content area.
         *
         * Loads either the home screen or settings screen based on mainWindow.isHomescreen.
         */
        Loader {
            height: ScreenUtils.scaledHeight(portraitMain.height, 926)
            width: ScreenUtils.scaledWidth(portraitMain.width)
            sourceComponent: mainWindow.isHomescreen ? homescreenCompP : settingsCompP
        }
        NavigationBar{
            id: navigationBarP
            height:ScreenUtils.scaledHeight(portraitMain.height, 124)
            width: ScreenUtils.scaledWidth(portraitMain.width)
            onIsMenuSelectedChanged: {
                mainWindow.isHomescreen = isMenuSelected
            }
        }
    }

    /**
     * @brief Home screen component for portrait mode.
     */
    Component {
        id: homescreenCompP
        Column {
            GaugesArea{
                id: gaugeArea
                height: ScreenUtils.scaledHeight(portraitMain.height, 260)
                width: ScreenUtils.scaledWidth(portraitMain.width)
                selectedIndex: widgetArea.selectedIndex

            }
            WidgetsArea{
                id: widgetArea
                anchors.horizontalCenter: parent.horizontalCenter
                height: ScreenUtils.scaledHeight(portraitMain.height, 666)
                width: ScreenUtils.scaledWidth(portraitMain.width)
            }
        }
    }

    /**
     * @brief Settings screen component for portrait mode.
     */
    Component {
        id: settingsCompP
        Menu {
            anchors.fill: parent
            anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
            anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
        }
    }

    ActionDialog {
        id: actionDialog
        anchors.centerIn: parent

    }
    // Component.onCompleted: {
    // actionDialog.open()}

    Connections {
        target: appInterface

        function onPopupTriggred() {
            console.log("opening popup:- ", appInterface.popup)

            switch (appInterface.popup) {

            case 10:
                openActionDialog(
                            "",
                            "Hydraulics enabled inhibited, Manual Catalyst Management is active",
                            "", "",false,false,[],false,true,Styles.color.pureWhite,false,"","",false,false
                            )
                break


            case 20:
                openActionDialog(
                            "Confirm Cancelation",
                            "Are you sure you want to cancel Manual Catalyst Management? Cancellation could cause damage to the system.To help reduce risk of damage, please allow time for cool down",
                            "Yes", "No",true,true,[],true,true,Styles.color.pureWhite,false,"","",false,false
                            )
                break

            case 30:
                openActionDialog(
                            "",
                            "Work The Machine Hard",
                            "Confirm", "",true,false,["qrc:/Images/infoIcon.svg"],false,false,Styles.color.popUpWarning,true,"","",false,false
                            )
                break
            case 40:
                openActionDialog(
                            "",
                            "Start the engine to check emission system performance.",
                            "Confirm", "",true,false,["qrc:/Images/stopIcon.svg"],true,false,Styles.color.popUpCriticalInfo,true,"Validation re-start remaining:        3","Time Remaining:     7min",true,true
                            )
                break
            case 50:
                openActionDialog(
                            "",
                            "Manual Catalyst Management is required now to avoid a dealer service call. Power reduction is active",
                            "Start Now ", "Postpone",true,true,["qrc:/Images/stopIcon.svg","qrc:/Images/stopIcon.svg","qrc:/Images/stopIcon.svg"],true,false,Styles.color.popUpCriticalInfo,true,"","",false,false
                            )
                break

            case 60:
                openActionDialog(
                            "",
                            "RTT Control Angle learnt",
                            "", "",false,false,[],false,true,Styles.color.pureWhite,false,"","",false,false
                            )
                break

            case 70:
                openActionDialog(
                            "",
                            "RTD Control Angle learnt",
                            "", "",false,false,[],false,true,Styles.color.pureWhite,false,"","",false,false
                            )
                break

            default:
                console.warn("Unknown popup type:", appInterface.popup)
                break
            }
        }
    }



    function openActionDialog(headerTxt, contentTxt, btn1Txt,btn2Tx,showBt1,showBt2,
                              imageList,contentLeftAlignment,isInfoPopup,borderColor
                              ,showHeaderImage,restartTxt,timeRemainTxt,showRestartTxt,showTimeRemainTxt){
        actionDialog.headerText = headerTxt
        actionDialog.contentText = contentTxt
        actionDialog.btn1Text = btn1Txt
        actionDialog.btn2Text = btn2Tx
        actionDialog.showBt1 = showBt1
        actionDialog.showBt2 = showBt2
        actionDialog.imageList = imageList
        actionDialog.contentLeftAlignment = contentLeftAlignment
        actionDialog.isInfoPopup = isInfoPopup
        actionDialog.borderColor = borderColor
        actionDialog.showHeaderImage = showHeaderImage
        actionDialog.restartText = restartTxt
        actionDialog.timeRemainText = timeRemainTxt
        actionDialog.showRestartText = showRestartTxt
        actionDialog.showTimeRemainText = showTimeRemainTxt
        actionDialog.open()
    }
}
