import QtQuick 2.12
import Styles 1.0

Item {
    Rectangle{
        anchors.fill: parent
        color: Styles.color.flatRect
        Image {
            id: logo
            height: 80
            width: 240
            anchors.centerIn: parent
            source: "qrc:/Images/CASE_logo.png"
        }
    }
}
