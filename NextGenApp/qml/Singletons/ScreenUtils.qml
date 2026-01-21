/**
 * @file ScreenUtils.qml
 * @brief Singleton Screen Utilities for scaling dimensions based on screen size and orientation.
 *
 * This singleton provides utility functions to scale heights and widths
 * according to the current screen orientation (portrait or landscape).
 * It defines reference dimensions and scaling functions to adapt UI elements
 * dynamically.

 * @date 11-Dec-2025
 * @author Malhar Sapatnekar
 */

pragma Singleton
import QtQuick 2.0

QtObject {
    id: _screenUtils
    property int portraitWidth: 414
    property int portraitHeight: 736

    property int landscapeWidth: 850
    property int landscapeHeight: 450

    property int referenceScreenHeight: 1280   // As per figma
    property int referenceScreenWidth: 720    // As per figma


    function scaledHeight(windowHeight, value=0) {
        if (value === 0 ){
            return windowHeight
        }
        else{
            return windowHeight * value / referenceScreenHeight
        }

    }


    function scaledWidth(windowWidth, value=0) {
        if (value === 0 ){
            return windowWidth
        }
        else{
            return windowWidth * value / referenceScreenWidth
        }


    }
}
