// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"

Page {
    id: roomDetailPage
    focus: true
    property string name: "RoomDetailPage"

    property Room room
    property int roomId: -2
    onRoomIdChanged: {
        if(roomId > 0) {
            room = dataManager.findRoomByRoomId(roomId)
            // dont forget to resolved if access sessions or floor/building
        }
    }

    Flickable {
        id: flickable
        property string name: "sessionDetail"
        // need some extra space if scrolling to bottom
        // and nothing covered by the FAB
        contentHeight: roomImage.height
        contentWidth: roomImage.width
        anchors.fill: parent

        Pane {
            id: root
            anchors.fill: parent

            Image {
                id: roomImage
                anchors.top: parent.top
                anchors.left: parent.left
                width: sourceSize.width
                height: sourceSize.height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/data-assets/conference/floorplan/room_"+room.roomId+".png"
            } // image

        } // root

    } // flickable



    FloatingActionMiniButton {
        visible: roomImage.scale >= 0.4
        property string imageName: "/remove.png"
        z: 1
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            roomImage.scale = roomImage.scale - 0.2
        }
    } // FAB
    FloatingActionMiniButton {
        visible: roomImage.scale <= 1.0
        property string imageName: "/add.png"
        z: 1
        anchors.leftMargin: 80
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            roomImage.scale = roomImage.scale + 0.2
        }
    } // FAB

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from RoomDetailPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from RoomDetailPage"))
    }

} // roomDetailPage
