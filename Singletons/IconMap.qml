pragma Singleton
import QtQuick

Item {
    id: _iconMap

    // font awesome ligature icon map
    property QtObject icons: QtObject {
        property string barcode: ""
        property string editFactors: ""
        property string certificate: ""
        property string proxEngaged: ""
        property string proxNotEngaged: ""
        property string provision: ""
        property string cameraWhite: ""
        property string spectrum : ""
        property string exportToday : ""
        property string back: ""
        property string factoryExit: ""
        property string sliderHandle: ""
        property string crossRemove: ""
        property string twoHand: ""
        property string barcodeGM: ""
    }
}
