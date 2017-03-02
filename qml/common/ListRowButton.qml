// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Button {
    id: button
    focusPolicy: Qt.NoFocus
    anchors.fill: parent
    contentItem: Text {
        text: ""
    }
    background:
        Rectangle {
        color: button.pressed ? Material.listHighlightColor : "transparent"
        radius: 2
    } // background
} // button
