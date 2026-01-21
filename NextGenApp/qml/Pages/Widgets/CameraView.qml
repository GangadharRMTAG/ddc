/**
 * @file CameraView.qml
 * @brief Simple camera view widget used to display camera image and provide access to camera settings.
 *
 * This QML component is a lightweight container that displays a camera image (currently a demo image)
 * and an overlaid settings button. It is intended to be used wherever a small camera preview and quick
 * access to camera settings are required.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
import QtQuick 2.12
import Styles 1.0
import ScreenUtils 1.0
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
        /**
         * @brief SafetyButton which opens camera settings.
         *
         * Positioned in the top-left corner of the camera view. Size is relative to the parent
         * rectangle to keep consistent layout across different resolutions.
         *
         */
        SafetyButton {
            id: cameraSettingsBtn
            height: ScreenUtils.scaledWidth(cameraView.width, 112)
            width:  ScreenUtils.scaledWidth(cameraView.width, 112)
            iconWidth: ScreenUtils.scaledWidth(cameraView.width, 55)
            iconHeight: ScreenUtils.scaledWidth(cameraView.width, 55)
            anchors {top: parent.top;left: parent.left;margins: height * 0.05}
            source: "qrc:/Images/Camera/CameraSettings.svg"
            showHighlightBar: false
        }
    }
}
