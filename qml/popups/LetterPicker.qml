// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import "../common"

Popup {
    id: popup
    closePolicy: Popup.CloseOnPressOutside
    property string titleText: "Select Letter"
    property bool isOK: false
    property string selectedLetter: "A"
    Material.elevation: 8

    x: appWindow.width - width
    width: Math.min(120, appWindow.height) / 3 * 2
    height: appWindow.height - 64
    transformOrigin: Popup.TopRight

    // Hint: as of Qt 5.7 Beta because of bug in dark theme,
    // background is too dark
    // https://bugreports.qt.io/browse/QTBUG-53266

    ListView {
        id: listView
        // Attention: clip should be used carefully,
        // but using a ListView inside a Popup
        // you must set it to true
        // otherwise content will appear outside while scrolling
        // don't clip at Paopup: will cut the elevation shadow
        clip: true

        currentIndex: -1
        anchors.fill: parent
        implicitHeight: popup.height
        delegate: Item {
            id: itemDelegate
            width: parent.width
            implicitHeight: 40
            Row {
                spacing: 0
                LabelTitle {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 24
                    text: model.letter
                    font.bold: true
                    color: primaryColor
                }
            } // end Row
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    popup.selectedLetter = model.letter
                    popup.isOK = true
                    popup.close()
                }
            } // mouse
        } // end delegateItem

        model: ListModel {
            ListElement { letter: "A" }
            ListElement { letter: "B" }
            ListElement { letter: "C" }
            ListElement { letter: "D" }
            ListElement { letter: "E" }
            ListElement { letter: "F" }
            ListElement { letter: "G" }
            ListElement { letter: "H" }
            ListElement { letter: "I" }
            ListElement { letter: "J" }
            ListElement { letter: "K" }
            ListElement { letter: "L" }
            ListElement { letter: "M" }
            ListElement { letter: "N" }
            ListElement { letter: "O" }
            ListElement { letter: "P" }
            ListElement { letter: "Q" }
            ListElement { letter: "R" }
            ListElement { letter: "S" }
            ListElement { letter: "T" }
            ListElement { letter: "U" }
            ListElement { letter: "V" }
            ListElement { letter: "W" }
            ListElement { letter: "X" }
            ListElement { letter: "Y" }
            ListElement { letter: "Z" }
        }
        ScrollIndicator.vertical: ScrollIndicator { }
    } // end listView
} // end popup
