// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ToolButton {
    Layout.alignment: Qt.AlignHCenter
    focusPolicy: Qt.NoFocus
    implicitHeight: 56
    implicitWidth: 56
    visible: appWindow.isComfortNavigationStyle
    Column {
        spacing: 0
        topPadding: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 24
            width: 24
            Image {
                height: 24
                width: 24
                verticalAlignment: Image.AlignTop
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/images/"+iconFolder+"/menu.png"
            }
        }
    }
    onClicked: {
        openNavigationBar()
        // see QTBUG-59293
        favMenuBugfix = !favMenuBugfix
    }
}
