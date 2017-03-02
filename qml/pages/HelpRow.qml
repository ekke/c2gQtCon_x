// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import "../common"

RowLayout {
    id: helpRow
    Layout.bottomMargin: 6
    property string iconName: ""
    property string helpText: ""
    Item {
        anchors.top: parent.top
        width: 24 + 32
        height: 24
        Image {
            id: myImage
            width: 24
            height: 24
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/"+iconFolder+"/"+helpRow.iconName
            transform: Translate {x: 16; y: 6}
        }
    } // image
    LabelSubheading {
        topPadding: 6
        leftPadding: 10
        rightPadding: 10
        wrapMode: Text.WordWrap
        text: helpRow.helpText
    }
} // helpRow
