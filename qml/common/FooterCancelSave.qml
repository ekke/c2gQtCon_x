// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Pane {
    property string buttonCANCEL: "CANCEL"
    property string buttonSAVE: "SAVE"
    property string buttonRESET: "RESET"
    property string buttonClicked: buttonRESET
    anchors.right: parent.right
    anchors.left: parent.left
    padding: 0
    z: 2
    opacity: 0.80
    ColumnLayout {
        anchors.right: parent.right
        anchors.left: parent.left
        RowLayout {
            spacing: 20
            Item {
                Layout.preferredWidth: 1
                Layout.fillWidth: true
            }
            ButtonFlat {
                Layout.preferredWidth: 1
                text: qsTr("Cancel")
                textColor: accentColor
                onClicked: {
                    buttonClicked = buttonCANCEL
                }
            }
            ButtonFlat {
                Layout.preferredWidth: 1
                text: qsTr("Save")
                textColor: primaryColor
                onClicked: {
                    buttonClicked = buttonSAVE
                }
            }
        } // row layout

    } // col layout
    function reset() {
        buttonClicked = buttonRESET
    }
} // pane


