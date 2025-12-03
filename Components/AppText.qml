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
