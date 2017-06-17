// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
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
        property string name: "roomDetail"
        // need some extra space if scrolling to bottom
        // and nothing covered by the FAB
        contentHeight: roomImage.sourceSize.height * roomImage.scale + 60
        contentWidth: roomImage.sourceSize.width * roomImage.scale
        anchors.fill: parent

            Image {
                id: roomImage
                anchors.top: parent.top
                anchors.left: parent.left
                width: sourceSize.width
                height: sourceSize.height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/data-assets/conference/floorplan/room_"+room.roomId+".png"
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
                transformOrigin: Item.TopLeft
                transform: Translate { y: 6; x:6 }
            } // image


    } // flickable

    FloatingActionMiniButton {
        visible: roomImage.scale >= 0.2
        property string imageName: "/remove.png"
        z: 1
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            roomImage.scale = roomImage.scale - 0.2
        }
    } // FAB
    FloatingActionMiniButton {
        property string imageName: "/aspect_ratio.png"
        z: 1
        anchors.leftMargin: 80
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            fitIntoWindow()
        }
    } // FAB
    FloatingActionMiniButton {
        visible: roomImage.scale <= 1.0
        property string imageName: "/add.png"
        z: 1
        anchors.leftMargin: 140
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            roomImage.scale = roomImage.scale + 0.2
        }
    } // FAB

    function fitIntoWindow() {
        var widthScale = (appWindow.width-20) / roomImage.sourceSize.width
        var heightScale = (appWindow.height-20) / roomImage.sourceSize.height
        roomImage.scale = Math.min(widthScale, heightScale)
        flickable.contentX = 0
        flickable.contentY = 0
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        fitIntoWindow()
        console.log("Init done from RoomDetailPage")
        appWindow.setDefaultTitleBarInLandscape()
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from RoomDetailPage")
        appWindow.resetDefaultTitleBarInLandscape()
    }

} // roomDetailPage
