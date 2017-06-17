// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    height:24
    Label {
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: 16
        font.weight: Font.Medium
        font.pixelSize: 14
        opacity: myBar.inactiveOpacity
        text: modelData.name
    }
}


