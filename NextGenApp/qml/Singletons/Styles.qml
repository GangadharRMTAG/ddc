/**
 * @file Styles.qml
 * @brief Singleton styles and theme definitions for NextGen Display UI.
 *
 * This QML file provides a singleton object containing color definitions, font loaders,
 * and other style-related properties used throughout the application.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
pragma Singleton
import QtQuick


Item {
    id: _styles

    // Colors
    property QtObject color: QtObject {
        //General
        property color lightBackground: "#F8FEFC"
        property color chooserBackground: "#F7F7F7"
        property color highlight: "#46A6D1"
        property color flatRect: "#113056"
        property color errorColor: "#FFD50000"
        property color transparent: "transparent"
        property color dottedLine: "#667895"
        property color fileIconOpen: "#00A3FF"
        property color valGreen: "#008900"
        property color charcolNavy: "#1b2736"
        property color charcolBlue: "#3b3f56"
        property color background: "#2B2E35"
        property color darkBackground: "#23252B"
        property color warningRed: "#ff0000"
        property color seperator: "#434860"
        property color pureWhite: "#ffffff"

        //Text
        property color textDark: "#061D3E"
        property color textDim: "#c7c9d7"
        property color textLight: "#FFF"
        property color textSelectionColor: "#5546A6D1"
        property color tabBarText: "#D0D3D4"
        property color tabBarTextSelected: "#052244"
        property color accHeadTextSelected: "#031D40"
        property color accHeadText: "#CFD3D6"
        property color pureBlack: "#000000"
        property color dropdownHeader: "#E1ECFF"
        property color warningOrange: "#F98600"

        //Component Backgrounds
        property color tabBarBackground: "#D2D2D2"
        property color resultViewBackground: "#58606C"
        property color periodicTableBackgroundColor: "#4F8E8F"
        property color dialogBackground: "#B2B6B7"
        property color toolTipBackground: "#E8213036"
        property color lavenderGray: "#D4D7E5"
        property color midnightBlue: "#0B0E18"
        property color darkSlate: "#2A304B"
        property color slateBlue: "#424761"
        property color mistBlue: "#626786"
        property color nightIndigo: "#211F30"
        property color mistLavender: "#C3C6D7"
        property color darkSlateBlue: "#32384f"
        property color steelNavyBlue: "#374058"
        property color lightGray: "#E0E0E0"
        property color edgeBlue:"#3098F2"

        // Pop up area
        property color popupBt1: "#D9DBE5"
        property color popupBt2: "#343640"
        property color popUpBg: "#191A1E"
        property color popUpWarning: "#FBA600"
        property color popUpCriticalInfo: "#FF4141"
    
        // Widget Button Area
        property color widgetButtonBackground: "#191A1E"
        property color widgetButtonPressed: "#D9DBE5"
     
        // Safety Area Buttons
        property color safetyAreaBtnActive: "#343640"

        // Flat button
        property color flatBtnHover: "#3A3E47"
        property color flatBtnPressed: "#1F2228"

        // Trip info widget
        property color tripInfoSeparator: "#606478" 


    }


    FontLoader {
        id: notoSansFont
        source: "qrc:/Fonts/NotoSans-Regular.ttf"
        onStatusChanged:
        if(notoSansFont.status === FontLoader.Error)
        console.error(notoSansFont.source)
    }

    FontLoader {
        id: notoMonoFont
        source: "qrc:/Fonts/NotoMono-Regular.ttf"
        onStatusChanged:
        if(notoMonoFont.status === FontLoader.Error)
        console.error(notoMonoFont.source)
    }

    // Fonts
    property QtObject font: QtObject {
        property string notoMono: notoMonoFont.name
        property string notoSans: notoSansFont.name
    }

    property QtObject text: QtObject{
        property string arrow : ">"
    }

}
