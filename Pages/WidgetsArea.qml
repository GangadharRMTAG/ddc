import QtQuick 2.12
import QtQuick.Shapes 1.12
import Styles 1.0
import "../Components"

Rectangle {
    id: widgetArea
    implicitWidth: 420
    implicitHeight: 420
    color: Styles.color.transparent

    property int value0to100: 35
    property int rpmValue: 1250
    property url source: ""
    property int selectedIndex: 0

    ListModel {
        id: buttonModel
        ListElement { icon: "qrc:/Images/WidgetArea/SpeedMeater.png"; page: "qrc:/Pages/Widgets/RpmWidget.qml"}
        ListElement { icon: "qrc:/Images/WidgetArea/TripInfo.png"; page: "qrc:/Pages/Widgets/TripInfo.qml"}
        ListElement { icon: "qrc:/Images/WidgetArea/CameraView.png"; page: "qrc:/Pages/Widgets/CameraView.qml" }
        ListElement { icon: "qrc:/Images/WidgetArea/Radio.png"; page: "qrc:/Pages/Widgets/Radio.qml" }
        ListElement { icon: "qrc:/Images/WidgetArea/Climate.png"; page: "qrc:/Pages/Widgets/Climate.qml" }
        ListElement { icon: "qrc:/Images/WidgetArea/MachineInfo.png"; page: "qrc:/Pages/Widgets/MachineInfo.qml" }
    }

    Row {
        id: buttonsRow
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Repeater {
            model: buttonModel
            delegate: SafetyButton {
                height: isPortrait ? widgetArea.height * 0.18 : widgetArea.height * 0.16
                width:  isPortrait ? widgetArea.width  * 0.18 : widgetArea.width  * 0.16
                showHighlightBar: false
                buttonHighlight : index === selectedIndex
                source: icon
                onClicked: {
                    selectedIndex = index
                    if(selectedIndex === 0){
                        widgetLoader.source = model.page
                    }else if(selectedIndex === 1){
                        widgetLoader.source = model.page
                    }else if(selectedIndex === 2){
                        widgetLoader.source = model.page
                    }else if(selectedIndex === 3){
                        widgetLoader.source = model.page
                    }else if(selectedIndex === 4){
                        widgetLoader.source = model.page
                    }else if(selectedIndex === 5){
                        widgetLoader.source = model.page
                    }
                }
            }
        }
    }

    Rectangle {
        id: widgetBackground
        x: 0
        y: buttonsRow.height
        width: isPortrait ? parent.width * 1.05 : parent.width
        height: parent.height - buttonsRow.height
        anchors.horizontalCenter: parent.horizontalCenter
        color: Styles.color.darkSlate
    }

    Loader {
        id: widgetLoader
        anchors.fill: widgetBackground
        source: "qrc:/Pages/Widgets/RpmWidget.qml"
        onLoaded: {
            item.value0to100 = widgetArea.value0to100
            item.rpmValue    = widgetArea.rpmValue
            item.source      = widgetArea.source
        }
    }
}
