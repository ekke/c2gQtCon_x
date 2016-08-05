import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import "../common"

Pane {
    property string name: "InitialItemPage"
    property int myIndex: -1
    width: rootPane.width
    LabelHeadline {
        id: initLabel
        anchors.left: parent.left
        anchors.right: parent.right
        topPadding: 12
        wrapMode: Label.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        text: qsTr("Welcome to\nekkes Conference2Go APP\nfor\nQtCon 2016")
    }
    BusyIndicator {
        id: busyIndicator
        topPadding: 24
        readonly property int size: Math.min(rootPane.availableWidth, rootPane.availableHeight) / 5
        width: size
        height: size
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: initLabel.bottom
    }
    LabelSubheading {
        id: infoLabel
        anchors.top: busyIndicator.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        wrapMode: Label.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        topPadding: 12
        color: primaryColor
    }
    Item {
        id: imageItem
        anchors.top: infoLabel.bottom
        property int size: Math.min(400, (rootPane.width-60))
        width: size
        Image {
            x: 24
            width: imageItem.size
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/extra/qt-con-logo.png"
        }
    }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }
    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from InitialItemPage"))
    }
    function showInfo(info) {
        console.log("INFO: "+info)
        infoLabel.text = info
    }

    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from InitialItemPage"))
    }
} // initialItem
