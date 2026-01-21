/**
 * @file AppText.qml
 * @brief Text component with styled fonts for NextGen Display UI.
 *
 * This QML file provides a reusable text component with predefined font styling.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */
import QtQuick
import Styles 1.0

Text {
    font.family: Styles.font.notoMono
    font.pixelSize: Styles.fontSize.item
    color: Styles.theme.pureBlack//Styles.color.textDark
    elide: Text.ElideRight
    minimumPixelSize: 14
    fontSizeMode: Text.Fit
    verticalAlignment: Text.AlignVCenter

}
