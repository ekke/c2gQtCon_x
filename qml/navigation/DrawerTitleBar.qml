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
        }
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
        }

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
            focusPolicy: Qt.NoFocus
            enabled: false
        }
    } // end RowLayout
} // end ToolBar


