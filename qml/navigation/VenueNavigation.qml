// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../pages"
import "../common"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "VenueNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "VenueNavPane"
        focus: true

        initialItem: VenuePage{
            id: initialItem
        }

        Loader {
            id: roomListPageLoader
            active: false
            visible: false
            source: "../pages/RoomListPage.qml"
            onLoaded: {
                navPane.push(item)
                item.init()
            }
        }
        Loader {
            id: roomDetailPageLoader
            property int roomId: -1
            active: false
            visible: false
            source: "../pages/RoomDetailPage.qml"
            onLoaded: {
                item.roomId = roomId
                navPane.push(item)
                item.init()
            }
        }


        function pushRoomListPage() {
            roomListPageLoader.active = true
        }

        function pushRoomDetail(roomId) {
            roomDetailPageLoader.roomId = roomId
            roomDetailPageLoader.active = true
        }

        function findPage(pageName) {
            var targetPage = find(function(item) {
                return item.name == pageName;
            })
            if(targetPage) {
                return targetPage.StackView.index
            } else {
                console.log("Page not found in StackView: "+pageName)
                return -1
            }
        }
        function backToPage(targetStackIndex) {
            for (var i=depth-1; i > targetStackIndex; i--) {
                popOnePage()
            }
        }

        function backToRootPage() {
            for (var i=depth-1; i > 0; i--) {
                popOnePage()
            }
        }

        function popOnePage() {
            var page = pop()
            if(page.name == "RoomListPage") {
                roomListPageLoader.active = false
                return
            }
//            if(page.name == "trackSessionListPage") {
//                trackSessionListPageLoader.active = false
//                return
//            }
//            if(page.name == "SpeakerDetailPage") {
//                speakerDetailPageLoader.active = false
//                return
//            }
//            if(page.name == "SessionDetailPage") {
//                sessionDetailPageLoader.active = false
//                return
//            }
            if(page.name == "RoomDetailPage") {
                roomDetailPageLoader.active = false
                return
            }

        } // popOnePage

    } // navPane

    FloatingActionButton {
        visible: navPane.depth == 1
        property string imageName: "/directions.png"
        z: 1
        anchors.rightMargin: 58
        anchors.bottomMargin: 6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.pushRoomListPage()
        }
    } // FAB
    FloatingActionButton {
        visible: navPane.depth == 2
        property string imageName: "/venue.png"
        z: 1
        anchors.rightMargin: 58
        anchors.bottomMargin: 6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.backToRootPage()
        }
    } // FAB
    FloatingActionButton {
        visible: navPane.depth > 2
        property string imageName: "/list.png"
        z: 1
        anchors.rightMargin: 58
        anchors.bottomMargin: 6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.backToPage(1)
        }
    } // FAB

    function destinationAboutToChange() {
        // nothing
    }

    // triggered from BACK KEYs:
    // a) Android system BACK
    // b) Back Button from TitleBar
    function goBack() {
        // check if goBack is allowed
        //
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT VenueNavPane")
        initialItem.init()
    }
    function cleanup() {
        console.log("CLEANUP VenueNavPane")
    }

} // navPage
