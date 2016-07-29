// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

Button {
    id: characterButton
    property int buttonSize: 24
    text: "S"
    font.pixelSize: buttonSize*3/4
    implicitWidth: buttonSize
    implicitHeight: buttonSize

    contentItem: Label {
        id: buttonLabel
        text: characterButton.text
        font: characterButton.font
        color: isDarkTheme? "black" : "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    } // content Label

    background: Rectangle {
        id: backgroundRectangle
        color: isDarkTheme? "lightgrey" : "darkgrey"
        radius: width / 2
    }
} // characterButton
