/**
 * @file WidgetsArea.qml
 * @brief Widget selection and display area for NextGen Display UI.
 *
 * This QML file provides a dynamic area for selecting and displaying various widgets
 * (RPM, Trip, Camera, Radio, Climate, Machine Info) using a button row and a loader.
 * It supports dynamic loading, property passing, and responsive design.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @updated Kunal Kokare
 */
import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.14
import Styles 1.0
import ScreenUtils 1.0
import "../Components"

Item{
    id: widgetArea

    /**
     * @property selectedIndex
     * @brief Index of the currently selected widget button.
     */
    property int selectedIndex: 0

    property string currentWidgetPage: ""

    /**
     * @brief Model for widget selection buttons.
     *
     * Each element contains:
     *   - name: Widget short name
     *   - icon: Icon image URL
     *   - page: QML file to load for the widget
     */
    ListModel {
        id: buttonModel
        ListElement { name: "RPM";   activeIcon: "qrc:/Images/WidgetArea/speedMeter_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/speedMeter_inactive.svg"   ;   page: "qrc:/Pages/Widgets/RpmWidget.qml"}
        ListElement { name: "TRP";   activeIcon: "qrc:/Images/WidgetArea/TripInfo_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/TripInfo_inactive.svg"      ;   page: "qrc:/Pages/Widgets/TripInfo.qml"}
        ListElement { name: "CAM";   activeIcon: "qrc:/Images/WidgetArea/CameraView_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/CameraView_inactive.svg"    ;   page: "qrc:/Pages/Widgets/CameraView.qml" }
        ListElement { name: "RAD";   activeIcon: "qrc:/Images/WidgetArea/media_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/media_inactive.svg"         ;   page: "qrc:/Pages/Widgets/Radio.qml" }
        ListElement { name: "CLI";   activeIcon: "qrc:/Images/WidgetArea/Climate_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/Climate_inactive.svg"       ;   page: "qrc:/Pages/Widgets/Climate.qml" }
        ListElement { name: "MCH";   activeIcon: "qrc:/Images/WidgetArea/MachineInfo_active.svg"; inactiveIcon: "qrc:/Images/WidgetArea/MachineInfo_inactive.svg"   ;   page: "qrc:/Pages/Widgets/MachineInfo.qml" }
    }

    Rectangle{
        color: Styles.color.background
        radius : ScreenUtils.scaledWidth(parent.width, 14)
        anchors.fill: parent
        anchors.topMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
        anchors.bottomMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
        anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
        anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
        Rectangle{
            id: widgetButtonArea
            height: ScreenUtils.scaledWidth(widgetArea.width, 102)
            color:  Styles.color.widgetButtonBackground
            radius : ScreenUtils.scaledWidth(parent.width, 16)
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
                leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
                rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
            }
            Row{
                anchors.fill: parent
                anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 10)
                anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 10)
                spacing: ScreenUtils.scaledWidth(parent.width, 12)
                Repeater {
                    model: buttonModel
                    delegate: SafetyButton {
                        height: ScreenUtils.scaledHeight(mainWindow.height, 86)
                        width: ScreenUtils.scaledWidth(widgetArea.width, 98.6)
                        iconWidth: ScreenUtils.scaledWidth(widgetArea.width, 64)
                        iconHeight: ScreenUtils.scaledWidth(widgetArea.width, 64)
                        showHighlightBar: false
                        btnColor: index === selectedIndex ? Styles.color.widgetButtonPressed : Styles.color.transparent
                        buttonHighlight : index === selectedIndex
                        source: index === selectedIndex ? activeIcon : inactiveIcon
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            selectedIndex = index
                        }
                    }
                }
            }
        }

        Rectangle {
            id: widgetBackground
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: widgetButtonArea.bottom
            anchors.topMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
            anchors.bottomMargin: ScreenUtils.scaledHeight(mainWindow.height, 14)
            anchors.leftMargin: ScreenUtils.scaledWidth(parent.width, 14)
            anchors.rightMargin: ScreenUtils.scaledWidth(parent.width, 14)
            color: Styles.color.background

            /**
             * @brief Loader for the selected widget page.
             *
             * Loads the QML file specified by the selected button. Calls loadPageVal on load.
             */
            Loader {
                id: widgetLoader
                anchors.fill: widgetBackground
                source: currentWidgetPage
            }
        }

    }
    function updateWidgetPage() {
        if (buttonModel.get(selectedIndex).name === "RPM") {
            currentWidgetPage = appInterface.creepActive
                    ? "qrc:/Pages/Widgets/RpmWidget.qml"
                    : "qrc:/Pages/Widgets/DefaultRpmWidget.qml"
        } else {
            currentWidgetPage = buttonModel.get(selectedIndex).page
        }
    }

    onSelectedIndexChanged: updateWidgetPage()
    Connections {
        target: appInterface
        function onCreepActiveChanged() {
            updateWidgetPage()
        }
    }

    Component.onCompleted: updateWidgetPage()

}
