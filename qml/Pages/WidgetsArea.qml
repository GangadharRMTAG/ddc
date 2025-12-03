
/**
 * @file WidgetsArea.qml
 * @brief Widget selection and display area for CASE CONSTRUCTION UI.
 *
 * This QML file provides a dynamic area for selecting and displaying various widgets
 * (RPM, Trip, Camera, Radio, Climate, Machine Info) using a button row and a loader.
 * It supports dynamic loading, property passing, and responsive design.
 */
import QtQuick 2.12
import QtQuick.Shapes 1.12
import Styles 1.0
import "../Components"

Item {
    id: widgetArea

    /**
     * @property selectedIndex
     * @brief Index of the currently selected widget button.
     */
    property int selectedIndex: 0


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
        ListElement { name: "RPM";   icon: "qrc:/Images/WidgetArea/SpeedMeater.png";      page: "qrc:/Pages/Widgets/RpmWidget.qml"}
        ListElement { name: "TRP";   icon: "qrc:/Images/WidgetArea/TripInfo.png";         page: "qrc:/Pages/Widgets/TripInfo.qml"}
        ListElement { name: "CAM";   icon: "qrc:/Images/WidgetArea/CameraView.png";       page: "qrc:/Pages/Widgets/CameraView.qml" }
        ListElement { name: "RAD";   icon: "qrc:/Images/WidgetArea/Radio.png";            page: "qrc:/Pages/Widgets/Radio.qml" }
        ListElement { name: "CLI";   icon: "qrc:/Images/WidgetArea/Climate.png";          page: "qrc:/Pages/Widgets/Climate.qml" }
        ListElement { name: "MCH";   icon: "qrc:/Images/WidgetArea/MachineInfo.png";      page: "qrc:/Pages/Widgets/MachineInfo.qml" }
    }

    Row {
        id: buttonsRow
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        spacing: widgetArea.width  * 0.02
        Repeater {
            model: buttonModel
            delegate: SafetyButton {
                height: widgetArea.height * 0.16
                width: widgetArea.width  * 0.15
                showHighlightBar: false
                buttonHighlight : index === selectedIndex
                source: icon
                onClicked: {
                    selectedIndex = index
                    widgetLoader.source = model.page
                }
                Rectangle {
                    width: parent.width
                    height: 6
                    anchors.bottom: parent.bottom
                    color:index === selectedIndex ? Styles.color.lavenderGray : Styles.color.charcolBlue
                }
            }
        }
    }

    Rectangle {
        id: widgetBackground
        x: 0
        y: buttonsRow.height
        width: parent.width
        height: parent.height - buttonsRow.height
        anchors.horizontalCenter: parent.horizontalCenter
        color: Styles.color.darkSlate

        /**
         * @brief Loader for the selected widget page.
         *
         * Loads the QML file specified by the selected button. Calls loadPageVal on load.
         */
        Loader {
            id: widgetLoader
            anchors.fill: widgetBackground
        }
    }

    Component.onCompleted: {
        widgetLoader.source = buttonModel.get(selectedIndex).page
    }
}
