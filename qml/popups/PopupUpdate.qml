// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import "../common"

Popup {
    id: updatePopup
    modal: true
    closePolicy: Popup.NoAutoClose
    property alias text: popupLabel.text
    property bool isUpdate: false
    property bool buttonsVisible: false
    property bool showUpdateButton: false
    Material.elevation: 8
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    implicitHeight: 160
    implicitWidth: parent.width * .9
    ColumnLayout {
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 20
        RowLayout {
            LabelTitle {
                id: popupLabel
                topPadding: 20
                leftPadding: 8
                rightPadding: 8
                text: ""
                color: popupTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            } // popupLabel
        } // row label
        RowLayout {
            visible: updatePopup.buttonsVisible
            spacing: 20
            Item {
                Layout.preferredWidth: 1
                Layout.fillWidth: true
            }
            ButtonFlat {
                Layout.preferredWidth: 1
                text: updatePopup.showUpdateButton? qsTr("Cancel") : qsTr("OK")
                textColor: accentColor
                onClicked: {
                    updatePopup.isUpdate = false
                    updatePopup.close()
                }
            }
            ButtonFlat {
                visible: updatePopup.showUpdateButton
                Layout.preferredWidth: 1
                text: qsTr("Update")
                textColor: primaryColor
                onClicked: {
                    updatePopup.isUpdate = true
                    updatePopup.close()
                }
            }
        } // row layout

    }
}
