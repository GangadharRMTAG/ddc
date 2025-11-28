import QtQuick
import Styles 1.0

Rectangle{
    id:curvedRectangle
    color: Styles.color.background
    Image {
        id: backgroundimg
        anchors.fill: parent
        source: "qrc:/Images/GaugesArea/Background.png"
    }
}
