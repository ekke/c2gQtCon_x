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
    contentHeight: appWindow.height
    contentWidth: appWindow.width
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "VenuePage"
    property Conference conference

    Pane {
        id: root
        anchors.fill: parent
        topPadding: 0
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
        Image {
            id: conferenceImage
            property real portraitScale: 1.0
            property real landscapeScale: 1.0
            scale: isLandscape? landscapeScale : portraitScale
            anchors.top: parent.top
            anchors.left: parent.left
            width: sourceSize.width
            height: sourceSize.height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/data-assets/conference/floorplan/building_1.png"
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
            transformOrigin: Item.TopLeft
        } // image

        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            transform: Translate {
                x: isLandscape ? conferenceImage.width*conferenceImage.scale:0
                y: isLandscape? 0 : conferenceImage.height*conferenceImage.scale
            } // translate

            RowLayout {
                LabelTitle {
                    topPadding: 16
                    leftPadding: 16
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: conference.address
                    color: primaryColor
                }
            }
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    function fitIntoWindow() {
        var portraitWidth = Math.min(appWindow.width,appWindow.height)
        var portraitHeight = Math.max(appWindow.width,appWindow.height)-60
        var portraitWidthScale = portraitWidth / conferenceImage.sourceSize.width
        var portraitHeightScale = portraitHeight / conferenceImage.sourceSize.height
        conferenceImage.portraitScale = Math.min(portraitWidthScale, portraitHeightScale)
        var landscapeWidth = Math.max(appWindow.width,appWindow.height)
        var landscapeHeight = Math.min(appWindow.width,appWindow.height)-80
        var landscapeWidthScale = landscapeWidth / conferenceImage.sourceSize.width
        var landscapeHeightScale = landscapeHeight / conferenceImage.sourceSize.height
        conferenceImage.landscapeScale = Math.min(landscapeWidthScale, landscapeHeightScale)
        flickable.contentX = 0
        flickable.contentY = 0
    }

    // called immediately after Loader.loaded
    function init() {
        conference = dataManager.findConferenceById(-1)
        fitIntoWindow()
        console.log("Init done from VenuePage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from VenuePage")
    }
} // flickable
