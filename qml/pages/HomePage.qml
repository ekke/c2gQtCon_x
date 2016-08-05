// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"

Flickable {
    id: homePage
    contentHeight: appWindow.height
    contentWidth: appWindow.width
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "HomePage"

    Pane {
        id: root
        anchors.fill: parent
        topPadding: 12
        width: appWindow.width
        Image {
            id: conferenceImage
            x: 24
            property real portraitScale: 1.0
            property real landscapeScale: 1.0
            scale: isLandscape? landscapeScale : portraitScale
            anchors.top: parent.top
            anchors.left: parent.left
            width: sourceSize.width
            height: sourceSize.height
            fillMode: Image.PreserveAspectFit
            source: isDarkTheme? "qrc:/images/extra/qt-con-logo-white.png":"qrc:/images/extra/qt-con-logo.png"
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
            transformOrigin: Item.TopLeft
        } // image

        ColumnLayout {
            Layout.fillWidth: true
            anchors.right: parent.right
            anchors.left: parent.left
            transform: Translate {
                x: isLandscape ? conferenceImage.width*conferenceImage.scale:0
                y: isLandscape? -22 : conferenceImage.height*conferenceImage.scale
            } // translate

            RowLayout {
                LabelHeadline {
                    Layout.maximumWidth: isLandscape? appWindow.width-12-(conferenceImage.width*conferenceImage.scale) : appWindow.width-12
                    topPadding: 24
                    leftPadding: 16
                    rightPadding: 16
                    wrapMode: Text.WordWrap
                    text: qsTr("Welcome to ekke's Conference2Go APP\nQtCon 2016, Berlin\n01. - 04. September")
                    color: accentColor
                }
            }
            RowLayout {
                LabelTitle {
                    Layout.maximumWidth: isLandscape? appWindow.width-12-(conferenceImage.width*conferenceImage.scale) : appWindow.width-12
                    topPadding: 36
                    leftPadding: 16
                    rightPadding: 16
                    wrapMode: Text.WordWrap
                    text: qsTr("This APP is developed with\nQt Quick Controls, Material Style")
                    color: primaryColor
                }
            }
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    LabelBodySecondary {
        anchors.leftMargin: 22
        anchors.bottomMargin: isLandscape? 64 : 116
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: qsTr("API Version ") + dataManager.settingsData().apiVersion
    }
    FloatingActionButton {
        property string imageName: "/upload.png"
        z: 1
        anchors.rightMargin: 58
        anchors.bottomMargin:isLandscape? 64 : isDarkTheme? 116 : 110
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            // dialog and start
        }
    } // FAB

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    function fitIntoWindow() {
        var portraitWidth = Math.min(appWindow.width,appWindow.height)
        var portraitHeight = Math.max(appWindow.width,appWindow.height)-60
        var portraitWidthScale = portraitWidth / conferenceImage.sourceSize.width
        var portraitHeightScale = portraitHeight / conferenceImage.sourceSize.height
        conferenceImage.portraitScale = Math.min(portraitWidthScale, portraitHeightScale) * 0.8
        var landscapeWidth = Math.max(appWindow.width,appWindow.height)
        var landscapeHeight = Math.min(appWindow.width,appWindow.height)-80
        var landscapeWidthScale = landscapeWidth / conferenceImage.sourceSize.width
        var landscapeHeightScale = landscapeHeight / conferenceImage.sourceSize.height
        conferenceImage.landscapeScale = Math.min(landscapeWidthScale, landscapeHeightScale) * 0.5
    }

    // called immediately after Loader.loaded
    function init() {
        fitIntoWindow()
        console.log(qsTr("Init done from Home Page"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from Home Page"))
    }
} // flickable
