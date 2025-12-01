import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Styles 1.0
import "../Components"

Column {
    id: settingsMenuRoot
    anchors.fill: parent
    spacing: 0

    property real designWidth: 360
    property real uiScale: Math.max(0.5, settingsMenuRoot.width / designWidth)
    property int selectedIndex: -1

    function loadPage(){
        if(selectedIndex === 0){
            portraitMain.pushAction("")
        }
    }

    Rectangle {
        id: menuBackground
        anchors.fill: parent
        color: Styles.color.background
        z: -1

        Rectangle {
            id: headerRect
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: 56 * settingsMenuRoot.uiScale
            color: Styles.color.charcolBlue

            Row {
                anchors.fill: parent
                anchors.leftMargin: 16 * settingsMenuRoot.uiScale
                spacing: 12 * settingsMenuRoot.uiScale

                Rectangle {
                    width: 24 * settingsMenuRoot.uiScale
                    height: 24 * settingsMenuRoot.uiScale
                    color: Styles.color.transparent
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        anchors.centerIn: parent
                        spacing: 4 * settingsMenuRoot.uiScale
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                        Rectangle { width: 24 * settingsMenuRoot.uiScale; height: 2 * settingsMenuRoot.uiScale; color: "#ffffff" }
                    }
                }

                Text {
                    text: "Menu"
                    font.pixelSize: Math.round(18 * settingsMenuRoot.uiScale)
                    font.weight: Font.Medium
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        ListModel {
            id: settingsModel
            ListElement { title: "Machine Service"; iconSource:  "qrc:/Images/SettingsPage/machineLerning.png" }
            ListElement { title: "Machine Settings"; iconSource: "qrc:/Images/SettingsPage/machineSettings.png" }
            ListElement { title: "Camera Settings"; iconSource: "qrc:/Images/SettingsPage/cameraSetting.png" }
            ListElement { title: "Trip Information"; iconSource: "qrc:/Images/SettingsPage/tripInformation.png" }
            ListElement { title: "Machine Status"; iconSource: "qrc:/Images/SettingsPage/machinestatus.png" }
            ListElement { title: "Phone"; iconSource: "qrc:/Images/SettingsPage/phone.png" }
            ListElement { title: "Lights"; iconSource: "qrc:/Images/SettingsPage/machinestatus.png" }
        }

        ListView {
            id: settingsListView
            anchors {
                left: parent.left
                right: parent.right
                top: headerRect.bottom
                bottom: parent.bottom
            }
            model: settingsModel
            spacing: 0
            clip: true

            delegate: Column {
                width: settingsListView.width
                spacing: 0

                FlatButton {
                    title: model.title
                    iconSource: model.iconSource
                    isArrow: true
                    onClicked: {
                        selectedIndex = index
                        console.log("Clicked:", title)
                        loadPage()
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                width: Math.max(3, Math.round(5 * settingsMenuRoot.uiScale))
                policy: ScrollBar.AsNeeded
                contentItem: Rectangle {
                    radius: height / 2
                    color: "#5a6270"
                    opacity: scrollBar.active || scrollBar.hovered ? 0.9 : 0.6
                    Behavior on opacity { NumberAnimation { duration: 200 } }
                }
                background: Rectangle { color: Styles.color.transparent }
                anchors.right: parent.right
                anchors.rightMargin: Math.round(2 * settingsMenuRoot.uiScale)
            }
        }
    }
}
