// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import "../common"

Popup {
    id: popup
    closePolicy: Popup.CloseOnPressOutside
    property bool isExit: false
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    implicitHeight: 160
    implicitWidth: parent.width * .9
    ColumnLayout {
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 10
        RowLayout {
            LabelSubheading {
                topPadding: 20
                leftPadding: 8
                rightPadding: 8
                text: qsTr("No more Pages\nDo you want to exit and leave the App ?\n\n")
                color: popupTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            } // popupLabel
        } // row label
        RowLayout {
            ButtonFlat {
                id: cancelButton
                text: qsTr("Cancel")
                textColor: primaryColor
                onClicked: {
                    isExit = false
                    popup.close()
                }
            } // cancelButton
            ButtonFlat {
                id: exitButton
                text: qsTr("Exit the App")
                textColor: accentColor
                onClicked: {
                    isExit = true
                    popup.close()
                }
            } // exitButton
        } // row button
    } // col layout

    onAboutToHide: {
        stopTimer()
    }
    onAboutToShow: {
        closeTimer.start()
    }

    Timer {
        id: closeTimer
        interval: 6000
        repeat: false
        onTriggered: {
            isExit = false
            popup.close()
        }
    }
    function stopTimer() {
        closeTimer.stop()
    }
} // popup
