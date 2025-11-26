import QtQuick 2.12
import Styles 1.0
import "./Pages"
import "./Components"

Item {

       anchors.fill: parent
       TopBar {
              id: topBarP
              height: parent.height / 10
              width: parent.width
              anchors{top: parent.top}
       }
       SafetyArea{
              id: safetyAreaP
              height:  parent.height /10
              width:   parent.width
              anchors{ top: topBarP.bottom;}
       }
}
