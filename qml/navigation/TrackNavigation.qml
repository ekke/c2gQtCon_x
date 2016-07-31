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
    property string name: "TrackNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "TrackNavPane"
        focus: true

        initialItem: Label{
            id: initialItem
            text: "Tracks"
            function init() {}
            function cleanup() {}
        }


    } // navPane

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
        console.log("INIT TrackNavPane")
        initialItem.init()
    }
    function cleanup() {
        console.log("CLEANUP TrackNavPane")
    }

} // navPage
