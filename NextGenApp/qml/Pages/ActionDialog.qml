import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Widgets"
import ScreenUtils 1.0
import Styles 1.0


Dialog {
    id: dialog
    modal: true

    width: ScreenUtils.scaledWidth(parent.width,650)
    implicitHeight: contentItem.implicitHeight
                    + footer.implicitHeight
                    + topPadding + bottomPadding

    property bool contentLeftAlignment: false
    property bool isInfoPopup: true
    property color borderColor: Styles.color.pureWhite
    property bool showHeaderImage: true
    property alias headerText: headerTxt.text
    property alias contentText: contentTxt.text
    property alias restartText: restartTxt.text
    property alias timeRemainText: timeRemainTxt.text
    property alias btn1Text: btn1.text
    property alias btn2Text: btn2.text
    property bool showBt1: true
    property bool showBt2: true
    property bool showRestartText: true
    property bool showTimeRemainText: true
    property var imageList: ["qrc:/Images/infoIcon.svg","qrc:/Images/infoIcon.svg","qrc:/Images/infoIcon.svg"]
    padding: 10

    Overlay.modal: Rectangle {
        color: Qt.rgba(0, 0, 0, 0.7)
    }

    contentItem: ColumnLayout {
        spacing: 6
        Layout.fillWidth: true

        Label {
            id :headerTxt
            text: headerText
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 20
            font.bold: true
            color: "white"
            visible: text !== ""&&!showHeaderImage
            Layout.alignment: Qt.AlignHCenter
        }

        Row{
            Layout.alignment: Qt.AlignHCenter
            visible: showHeaderImage
            spacing: 3

            Repeater{
                model: imageList
                Image {
                    source:modelData
                    fillMode: Image.PreserveAspectFit
                    width:32//ScreenUtils.scaledWidth(parent.width,64)
                    height:32//ScreenUtils.scaledWidth(parent.width,64)
                }
            }

        }
        Label {
            id: contentTxt
            text:contentText
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 14
            font.weight: Font.Normal
            color: "#DDDDDD"
            Layout.fillWidth: true
            lineHeight: 1.3
            visible: true
            horizontalAlignment: contentLeftAlignment?Qt.AlignLeft:Qt.AlignHCenter
        }
        Label {
            id: restartTxt
            text: restartText
            font.pixelSize: 14
            wrapMode: Text.WrapAnywhere
            font.weight: Font.Normal
            color: "#DDDDDD"
            Layout.fillWidth: true
            lineHeight: 1.3
            visible: showRestartText
            Layout.alignment: Qt.AlignLeft
        }
        Label {
            id: timeRemainTxt
            text: timeRemainText
            font.pixelSize: 14
            wrapMode: Text.WrapAnywhere
            font.weight: Font.Normal
            color: "#DDDDDD"
            Layout.fillWidth: true
            lineHeight: 1.3
            visible: showTimeRemainText
            Layout.alignment: Qt.AlignLeft
        }

        ColumnLayout{
            Layout.alignment: Qt.AlignHCenter
            spacing: 6
            visible: showBt1||showBt2
            DialogButton {
                id: btn1
                width: 301//ScreenUtils.scaledWidth(parent.width,602)
                height: 40//ScreenUtils.scaledHeight(portraitMain.height,80)
                visible: showBt1
                text: ""
                onClicked: dialog.accept()
                color: Styles.color.popupBt1
                textColor: Styles.color.popUpBg

            }

            DialogButton {
                id: btn2
                width: 301//ScreenUtils.scaledWidth(parent.width,602)
                height: 40//ScreenUtils.scaledHeight(portraitMain.height,80)
                visible: showBt2
                text: ""
                onClicked: dialog.reject()
                color: Styles.color.popupBt2
                textColor: Styles.color.pureWhite
            }
        }
    }


    background: Rectangle {
        color: Styles.color.popUpBg
        radius: 10
        border.color: borderColor
        border.width: isInfoPopup?ScreenUtils.scaledWidth(parent.width,3):ScreenUtils.scaledWidth(parent.width,6)
    }
}
