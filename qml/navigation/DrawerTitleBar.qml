// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import "../common"

ToolBar {
    id: myTitleBar

    RowLayout {
        focus: false
        spacing: 6
        anchors.fill: parent
        Item {
            width: 4
        }

        ToolButton {
            visible: !backButton.visible
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/menu.png"
            }
            onClicked: {
                appWindow.openNavigationBar()
            }
        } // menubutton
        ToolButton {
            id: backButton
            focusPolicy: Qt.NoFocus
            visible: initDone && navigationModel[navigationIndex].canGoBack && destinations.itemAt(navigationIndex).item.depth > 1
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/arrow_back.png"
            }
            onClicked: {
                destinations.itemAt(navigationIndex).item.goBack()
            }
        } // backButton

        LabelTitle {
            id: titleLabel
            text: currentTitle
            leftPadding: 6
            rightPadding: 6
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: textOnPrimary
        }
        ToolButton {
            id: scheduleGoToButton
            visible: navigationIndex == 2 && destinations.itemAt(2).item.depth == 1
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/goto.png"
            }
            onClicked: {
                destinations.itemAt(2).item.pickTime()
            }
        } // scheduleGoToButton
        // fake button to avoid flicker and repositioning of titleLabel
        ToolButton {
            visible: !scheduleGoToButton.visible
            enabled: false
            focusPolicy: Qt.NoFocus
        } // fake button
    } // end RowLayout
} // end ToolBar


