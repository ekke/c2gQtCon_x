// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0
import "../common"

ToolButton {
    id: myButton
    property bool isActive: index == navigationIndex
    property string myIconFolder: iconFolder
    property int counter: navigationData[index].counter
    property color marker: navigationData[index].marker
    property string theIcon: modelData.icon
    property string theText: modelData.name
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignHCenter
    focusPolicy: Qt.NoFocus
    height: 48
    width: myBar.width

    // Material.buttonPressColor
    Rectangle {
        visible: highlightActiveNavigationButton && myButton.isActive
        height: myButton.height
        width: myButton.width
        color:  Material.listHighlightColor
    }

    Row {
        spacing: 0
        topPadding: 0
        leftPadding: 16
        rightPadding: modelData.showCounter? 24 :  16
        anchors.verticalCenter: parent.verticalCenter
        Item {
            anchors.verticalCenter: parent.verticalCenter
            width: 24 + 32
            height: 24
            Image {
                id: myImage
                width: 24
                height: 24
                horizontalAlignment: Image.AlignLeft
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/"+myIconFolder+"/"+theIcon
                opacity: isActive? myBar.activeOpacity : myBar.inactiveOpacity
            }
            ColorOverlay {
                id: colorOverlay
                visible: myButton.isActive
                anchors.fill: myImage
                source: myImage
                color: primaryColor
            }
        } // image and coloroverlay
        Label {
            id: buttonLabel
            anchors.verticalCenter: parent.verticalCenter
            text: theText
            opacity: 0.87 // isActive? 1.0 : 0.7
            color: isActive? primaryColor : dividerColor // flatButtonTextColor
            font.pixelSize: 14
            font.weight: Font.Medium
        } // label
    } // row
    Label {
        rightPadding: 16
        visible: modelData.showCounter
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        text: counter
        opacity: 0.87 // isActive? 1.0 : 0.7
        color: isActive? primaryColor : dividerColor // flatButtonTextColor
        font.pixelSize: 14
        font.weight: Font.Medium
    } // label
    Rectangle {
        visible: modelData.showMarker
        anchors.right: parent.right
        width: 6
        height: parent.height
        color: marker
    }
    onClicked: {
        navigationIndex = index
        closeNavigationBar()
    }
} // myButton
