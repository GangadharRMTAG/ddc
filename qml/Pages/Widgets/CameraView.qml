import QtQuick 2.12
import Styles 1.0
import "../../Components"

Item {
    id: cameraView

    Rectangle{
        id: cameraViewRect
        anchors.fill: parent
        color: Styles.color.transparent
        Image {
            id: cameraImg
            anchors.fill: parent
            source: "qrc:/Images/Camera/DemoCameraImg.png"
        }
        SafetyButton {
            id: cameraSettingsBtn
            height: cameraViewRect.height * 0.2
            width: cameraViewRect.width * 0.15
            anchors {top: parent.top;left: parent.left;margins: height * 0.05}
            source: "qrc:/Images/Camera/CameraSettings.png"
            showHighlightBar: false
        }
    }
}
