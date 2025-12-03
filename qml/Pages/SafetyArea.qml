/**
 * @file SafetyArea.qml
 * @brief Safety area indicator and control bar for CASE CONSTRUCTION UI.
 *
 * This QML file provides a responsive safety area with status buttons, supporting both
 * portrait and landscape layouts. Icons are dynamically generated from a model and can
 * display different images when selected.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Styles 1.0
import "../Components"

Item {
    id: safetyArea

    /**
     * @brief Model for safety area status buttons.
     *
     * Each element contains:
     *   - icon: Icon image URL (default)
     *   - selectedIcon: Icon image URL when selected
     *
     * Empty elements are placeholders for future buttons.
     */
    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/SafetyArea/iso.png";   selectedIcon: "qrc:/Images/SafetyArea/H.png" }
        ListElement { icon: "qrc:/Images/SafetyArea/bdsl.png";  selectedIcon: "qrc:/Images/SafetyArea/bdsl.png" }
        ListElement { icon: "qrc:/Images/SafetyArea/creep.png"; selectedIcon: "qrc:/Images/SafetyArea/creep.png" }
        ListElement { icon: "" ;                                selectedIcon: ""}
        ListElement { icon: "" ;                                selectedIcon: ""}
        ListElement { icon: "" ;                                selectedIcon: ""}
    }


    /**
     * @brief Loader for switching between portrait and landscape layouts.
     *
     * Loads the appropriate layout based on the isPortrait property.
     */
    Loader {
        id: orientationLoader
        anchors.centerIn: parent
        active: true
        sourceComponent: isPortrait ?  portraitLayout : landscapeLayout
    }


    /**
     * @brief Portrait layout for the safety area.
     */
    Component {
        id: portraitLayout
        Row {
            anchors.centerIn: parent
            spacing: safetyArea.width * 0.01
            Repeater {
                model: buttonModel
                SafetyButton {
                    height: safetyArea.height
                    width:  safetyArea.width * 0.15
                    source:  selected ? model.selectedIcon :  model.icon
                }
            }
        }
    }


    /**
     * @brief Landscape layout for the safety area.
     */
    Component {
        id: landscapeLayout
        Column {
            anchors.centerIn: parent
            Repeater {
                model: buttonModel
                SafetyButton {
                    height: safetyArea.height * 0.16
                    width:  safetyArea.width
                    source:  selected ? model.selectedIcon :  model.icon
                }
            }
        }
    }
}
