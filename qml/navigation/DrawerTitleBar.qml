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
            visible: !backButton.visible && (appWindow.isLandscape || !appWindow.hasOnlyOneMenu)
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/menu.png"
            }
            onClicked: {
                appWindow.openNavigationBar()
            }
        } // menu button
        // F A K E
        // fake button to avoid flicker and repositioning of titleLabel
        ToolButton {
            visible: !backButton.visible && appWindow.hasOnlyOneMenu && !appWindow.isLandscape
            enabled: false
            focusPolicy: Qt.NoFocus
        } // fake button
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
            visible: !myScheduleActive && navigationIndex == scheduleNavigationIndex && destinations.itemAt(scheduleNavigationIndex).item.depth == 1
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/goto.png"
            }
            onClicked: {
                destinations.itemAt(scheduleNavigationIndex).item.pickTime()
            }
        } // scheduleGoToButton
        ToolButton {
            id: speakerGoToButton
            visible: navigationIndex == speakerNavigationIndex && destinations.itemAt(speakerNavigationIndex).item.depth == 1
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/goto.png"
            }
            onClicked: {
                destinations.itemAt(3).item.pickLetter()
            }
        } // scheduleGoToButton
        ToolButton {
            id: homeOptionsButton
            visible: navigationIndex == homeNavigationIndex
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/more_vert.png"
            }
            onClicked: {
                homeOptionsMenu.open()
            }
            Menu {
                id: homeOptionsMenu
                x: parent.width - width
                transformOrigin: Menu.TopRight
                MenuItem {
                    text: qsTr("Help")
                    onTriggered: {
                        navigationIndex = helpNavigationIndex
                    }
                }
                MenuItem {
                    text: qsTr("Settings")
                    onTriggered: {
                        navigationIndex = settingsNavigationIndex
                    }
                }
                MenuItem {
                    text: qsTr("About")
                    onTriggered: {
                        navigationIndex = aboutNavigationIndex
                    }
                }
                onAboutToHide: {
                    appWindow.resetFocus()
                }
            } // end optionsMenu

        } // end homeOptionsButton

        // F A K E
        // fake button to avoid flicker and repositioning of titleLabel
        ToolButton {
            visible: !scheduleGoToButton.visible && !speakerGoToButton.visible && !homeOptionsButton.visible
            enabled: false
            focusPolicy: Qt.NoFocus
        } // fake button
    } // end RowLayout
} // end ToolBar


