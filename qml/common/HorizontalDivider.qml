// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    height: 8
    anchors.left: parent.left
    anchors.right: parent.right
    // anchors.margins: 6
    // https://www.google.com/design/spec/components/dividers.html#dividers-types-of-dividers
    Rectangle {
        width: parent.width
        height: 1
        opacity: dividerOpacity
        color: dividerColor
    }
}
