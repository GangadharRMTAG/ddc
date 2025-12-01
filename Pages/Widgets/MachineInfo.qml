import QtQuick 2.12
import Styles 1.0

Item {
    id: machineInfo

    ListModel {
        id: machineInformation
        ListElement { title: "Machine";           value: "Titan machinery";}
        ListElement { title: "Operator";          value: "Michael M.";}
        ListElement { title: "Service";           value: "+01 12345 67890";}
        ListElement { title: "Maintenance due";   value: "256h";}
    }

    Rectangle{
        anchors.fill: parent
        color: Styles.color.transparent
        Column{
            anchors.fill: parent
            spacing: 1
            Repeater {
                model: machineInformation
                delegate: Rectangle {
                    height: machineInfo.height * 0.22
                    width: machineInfo.width
                    color: Styles.color.darkSlate
                    Text {
                        id: headingText
                        text: model.title
                        font{pixelSize: 17;family: Styles.font.notoSans}
                        color: Styles.color.textLight
                        anchors{verticalCenter: parent.verticalCenter;left: parent.left;leftMargin: parent.width * 0.05}
                    }
                    Text {
                        id: valueText
                        text: model.value
                        font{pixelSize: 17;family: Styles.font.notoSans}
                        color: Styles.color.textDim
                        anchors{verticalCenter: parent.verticalCenter;right: parent.right;rightMargin: parent.width * 0.05}
                    }
                    Rectangle{
                        height: 1
                        width: machineInfo.width * 0.9
                        color: Styles.color.seperator
                        anchors{bottom: parent.bottom;horizontalCenter: parent.horizontalCenter}
                    }
                }
            }
        }
    }
}
