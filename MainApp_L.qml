import QtQuick 2.12
import "./Pages"
import "./Components"

Item {

    anchors.fill: parent
    TopBar{
        id: topBarL
        height: parent.height
        width:  parent.width /10
        anchors{left: parent.left}
    }

    SafetyArea{
           id: safetyAreaL
           height:  parent.height
           width:   parent.width/10
           anchors{left: topBarL.right;}
    }
}
