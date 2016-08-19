// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    property string name: "SettingsPage"
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    clip: true

    Pane {
        id: root
        anchors.fill: parent

        ColumnLayout {
            id: theContent
            anchors.right: parent.right
            anchors.left: parent.left
            RowLayout {
                Image {
                    source: "qrc:/images/extra/qtcon-app.png"
                }
                LabelHeadline {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Conference2Go\nQtCon 2016, Berlin\n(Qt 5.7 - Android + iOS)")
                    color: primaryColor
                }
            }
            HorizontalDivider {}
            RowLayout {
                LabelBodySecondary {
                    topPadding: 6
                    leftPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Dark Theme")
                }
                Switch {
                    topPadding: 6
                    anchors.right: parent.right
                    focusPolicy: Qt.NoFocus
                    checked: isDarkTheme
                    onCheckedChanged: {
                        if(checked != isDarkTheme) {
                            themePalette = myApp.swapThemePalette()
                        }
                    }
                } // switch darktheme
            } // row switch dar theme
            RowLayout {
                LabelBodySecondary {
                    id: labelPrimary
                    topPadding: 6
                    leftPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Primary Color:\nTap on Color Rectangle to edit")
                }
                Rectangle {
                    anchors.verticalCenter: labelPrimary.verticalCenter
                    anchors.right: parent.right
                    width: 100
                    height: 40
                    color: primaryColor
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            navPane.pushPrimaryColorPage()
                        }
                    } // mouse
                }
            } // row primary
            Item {
                height: 6
            }
            RowLayout {
                LabelBodySecondary {
                    id: labelAccent
                    topPadding: 6
                    leftPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Accent Color:\nTap on Color Rectangle to edit")
                }
                Rectangle {
                    anchors.verticalCenter: labelAccent.verticalCenter
                    anchors.right: parent.right
                    width: 100
                    height: 40
                    color: accentColor
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            navPane.pushAccentColorPage()
                        }
                    } // mouse
                }
            } // row accent
            HorizontalDivider {}
            RowLayout {
                LabelBodySecondary {
                    Layout.preferredWidth: 1
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Default Theme:")
                }
                LabelBody {
                    Layout.preferredWidth: 2
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Light Theme\nPrimary Color: Indigo\nAccent Color: Deep Orange")
                }
            }
            RowLayout {
                LabelBodySecondary {
                    Layout.preferredWidth: 1
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Hint:")
                }
                LabelBody {
                    Layout.preferredWidth: 2
                    topPadding: 6
                    bottomPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Switching to Dark Theme please change Primary Color to Blue, Green or so. Indigo itself is too dark.")
                }
            }
            HorizontalDivider {}
            RowLayout {
                LabelBodySecondary {
                    Layout.preferredWidth: 1
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Version:")
                }
                LabelBody {
                    Layout.preferredWidth: 2
                    topPadding: 6
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: dataManager.settingsData().version
                }
            }
        } // col layout

    } // root

    ScrollIndicator.vertical: ScrollIndicator { }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SettingsPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SettingsPage"))
    }

} // flickable
