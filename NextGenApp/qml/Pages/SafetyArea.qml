/**
 * @file SafetyArea.qml
 * @brief Safety area indicator and control bar for NextGen Display UI.
 *
 * This QML file provides a responsive safety area with status buttons, supporting both
 * portrait and landscape layouts. Icons are dynamically generated from a model and can
 * display different images when selected.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @modified Kunal Kokare
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Styles 1.0
import ScreenUtils 1.0
import "../Components"

Item {
    id: safetyArea
    property int selectedIndex: -1
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
        ListElement { icon: "qrc:/Images/SafetyArea/iso.svg";   selectedIcon: "qrc:/Images/SafetyArea/H.svg" }
        ListElement { icon: "qrc:/Images/SafetyArea/creep.svg"; selectedIcon: "qrc:/Images/SafetyArea/creep.svg" }
        ListElement { icon: "qrc:/Images/SafetyArea/Union.svg"; selectedIcon: "qrc:/Images/SafetyArea/Union.svg" }
        ListElement { icon: "qrc:/Images/SafetyArea/Union.svg"; selectedIcon: "qrc:/Images/SafetyArea/Union.svg"}
        ListElement { icon: "qrc:/Images/SafetyArea/Union.svg"; selectedIcon: "qrc:/Images/SafetyArea/Union.svg"}
        ListElement { icon: "qrc:/Images/SafetyArea/Union.svg"; selectedIcon: "qrc:/Images/SafetyArea/Union.svg"}
    }


    /**
     * @brief Loader for switching between portrait and landscape layouts.
     *
     * Loads the appropriate layout based on the isPortrait property.
     */
    Loader {
        id: orientationLoader
        anchors.fill: parent
        active: true
        sourceComponent: isPortrait ?  portraitLayout : landscapeLayout
    }


    /**
     * @brief Portrait layout for the safety area.
     */
    Component {
        id: portraitLayout
        Row {
            anchors.fill: parent
            anchors.leftMargin: ScreenUtils.scaledWidth(safetyArea.width, 14)
            anchors.rightMargin: ScreenUtils.scaledWidth(safetyArea.width, 14)
            anchors.topMargin: ScreenUtils.scaledWidth(safetyArea.width, 14)
            anchors.bottomMargin: ScreenUtils.scaledWidth(safetyArea.width, 14)
            spacing: ScreenUtils.scaledWidth(safetyArea.width, 4)
            Repeater {
                model: buttonModel
                SafetyButton {
                    property int buttonIndex: index
                    height: ScreenUtils.scaledWidth(safetyArea.width, 112)
                    width:  ScreenUtils.scaledWidth(safetyArea.width, 112)
                    source:  selected ? model.selectedIcon :  model.icon
                    iconWidth: ScreenUtils.scaledWidth(safetyArea.width, 55)
                    iconHeight: ScreenUtils.scaledWidth(safetyArea.width, 55)

                    showHighlightBar: (buttonIndex === 0 || buttonIndex === 1)
                    /* Central selection handling */
                    selected:
                        buttonIndex === 0 ? appInterface.isoActive :
                        buttonIndex === 1 ? appInterface.creepActive :
                        safetyArea.selectedIndex === buttonIndex


                    onClicked: {
                        safetyArea.selectedIndex =
                                (safetyArea.selectedIndex === buttonIndex) ? -1 : buttonIndex

                        handleSafetyButton(buttonIndex, selected)
                    }

                    onSelectedChanged: {
                        console.log(
                                    "[SafetyArea]",
                                    "Index:", buttonIndex,
                                    "Selected:", selected,
                                    "Icon:", selected ? model.selectedIcon : model.icon
                                    )
                    }


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


    /* ---------------- CAN / LOGIC ---------------- */
    function handleSafetyButton(index, state) {
        switch (index) {

        case 0: // ISO
            console.log("ISO Button ->", state ? "H Enabled" : "ISO Enabled")
            appInterface.publishButtonStatus(0, state)


            break

        case 1:// Creep
            console.log("Creep Button ->", state ? "Enabled" : "Disabled")
            appInterface.publishButtonStatus(2, state)
            appInterface.setCreepActive(state)
            break

        case 2:


        default:
            console.log("No mapping for button index:", index)
            break
        }
    }

}
