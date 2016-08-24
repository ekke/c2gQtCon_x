// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"

Flickable {
    id: flickable
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "DevToolsPage"

    Pane {
        id: root
        property SettingsData settingsData
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            LabelSubheading {
                topPadding: 12
                bottomPadding: 12
                leftPadding: 10
                text: qsTr("Some special Properties while DEBUG mode is ON")
                color: primaryColor
            }
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Public Cache")
                    Layout.preferredWidth: 1
                }
                Item {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    Layout.preferredHeight: publicCacheSwitch.implicitHeight
                    Switch {
                        id: publicCacheSwitch
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                        rightPadding: 10
                        checked: root.settingsData.hasPublicCache
                        onCheckedChanged: {
                            root.settingsData.hasPublicCache = checked
                            if(root.settingsData.publicRoot4Dev.length == 0) {
                                root.settingsData.publicRoot4Dev = "/data/ekkescorner/"+Qt.application.name.replace("lib","").replace(".so","")
                            }
                        }
                    } // switch
                } // switch item
            } // row
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("normal cache: QStandardPaths::AppDataLocation\npublic cache: QStandardPaths::GenericDataLocation")
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: root.settingsData.publicRoot4Dev
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
            HorizontalDivider {}

            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Compact JSON Format")
                    Layout.preferredWidth: 1
                }
                Item {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    Layout.preferredHeight: compactJsonSwitch.implicitHeight
                    Switch {
                        id: compactJsonSwitch
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                        rightPadding: 10
                        checked: root.settingsData.useCompactJsonFormat
                        onCheckedChanged: {
                            root.settingsData.useCompactJsonFormat = checked
                        }
                    } // switch
                } // switch item
            } // row
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Without DEBUG mode COMPACT JSON is always used. JSON with Indents is better readable, so you can switch this for DEBUG mode.")
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
            HorizontalDivider {}
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("You must restart to enable dev tools settings")
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
            HorizontalDivider {}
            LabelHeadline {
                leftPadding: 10
                text: qsTr("Prepare the Conference Data")
            }
            RowLayout {
                ButtonFlat {
                    text: qsTr("prepare")
                    textColor: primaryColor
                    onClicked: {
                        dataUtil.prepareConference()
                    }
                }
            }
            // "/storage/emulated/0/data/ekkescorner/c2gQtCon_x/data/conference/"
            // "/storage/emulated/0/data/ekkescorner/c2gQtCon_x/data/conference/speakerImages/"
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Prepare Conference must run with public cache.\nWhile running watch debug log in QtCreator.\nIf all went well data can be delivered as qrc:/data-assets.\nMore info inside src.")
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
            RowLayout {
                Layout.leftMargin: 16
                LabelBodySecondary {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WrapAnywhere
                    text: qsTr("Conference data stored at:\n/storage/emulated/0/data/ekkescorner/c2gQtCon_x/data/conference/")
                    Layout.preferredWidth: 1
                    font.italic: true
                }
            } // row
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        root.settingsData = dataManager.settingsData()
        console.log("Init done from DEV TOOLS")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from DEV TOOLS")
    }
} // flickable
