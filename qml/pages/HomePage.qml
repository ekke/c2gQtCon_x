// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"
import "../popups"

Pane {
    id: homePage
    height: appWindow.height
    property string name: "HomePage"
    property Conference conference
    topPadding: 12
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
                topPadding: 8
                leftPadding: 16
                rightPadding: 16
                wrapMode: Text.WordWrap
                text: qsTr("QtCon 2016, Berlin\n01. - 04. September")
                color: primaryColor
            }
        }
        RowLayout {
            LabelHeadline {
                Layout.maximumWidth: isLandscape? appWindow.width-12-(conferenceImage.width*conferenceImage.scale) : appWindow.width-12
                topPadding: 12
                leftPadding: 16
                rightPadding: 16
                wrapMode: Text.WordWrap
                text: qsTr("Welcome to ekke's Conference2Go app")
                color: accentColor
            }
        }
        RowLayout {
            LabelTitle {
                Layout.maximumWidth: isLandscape? appWindow.width-12-(conferenceImage.width*conferenceImage.scale) : appWindow.width-12
                topPadding: 16
                leftPadding: 16
                rightPadding: 16
                wrapMode: Text.WordWrap
                text: qsTr("Developed with Qt 5.7\nQt Quick Controls 2\n(Material Style)")
                color: primaryColor
            }
        }
        RowLayout {
            LabelTitle {
                id: hashtagLabel
                Layout.maximumWidth: isLandscape? appWindow.width-12-(conferenceImage.width*conferenceImage.scale) : appWindow.width-12
                topPadding: 10
                leftPadding: 16
                rightPadding: 16
                wrapMode: Text.WordWrap
                text: conference? "Twitter " + conference.hashTag : ""
                color: accentColor
            }
        }
    } // col layout

    LabelBodySecondary {
        anchors.leftMargin: 22
        anchors.bottomMargin: isLandscape || appWindow.isClassicNavigationStyle? 48 : 100
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: dataUtil.apiInfo()
    }
    FloatingActionButton {
        property string imageName: "/refresh.png"
        z: 1
        anchors.margins: 8
        anchors.bottomMargin: isLandscape || appWindow.isClassicNavigationStyle? 60:112
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            // check if date is OK
            if(dataUtil.isDateTooLate()) {
                appWindow.showToast(qsTr("Sorry - the Conference is closed.\nNo more Updates available"))
                return
            }

            // open modal dialog and wait if update required
            updatePopup.text = qsTr("Checking QtCon Server\nfor new Schedule Data ...")
            updatePopup.buttonsVisible = false
            updatePopup.isUpdate = false
            updatePopup.open()
        }
    } // FAB

    PopupUpdate {
        id: updatePopup
        modal: true
        closePolicy: Popup.NoAutoClose
        onOpened: {
            dataUtil.checkVersion()
        }
        onClosed: {
            if(updatePopup.isUpdate) {
                rootPane.startUpdate()
            }
        }
    } // updatePopup

    function updateAvailable(apiVersion) {
        console.log("QML updateAvailable " + apiVersion)
        updatePopup.text = qsTr("Update available.\nAPI Version: ")+apiVersion
        updatePopup.showUpdateButton = true
        updatePopup.buttonsVisible = true

    }
    function noUpdateRequired() {
        console.log("QML noUpdateRequired")
        updatePopup.text = qsTr("No Update required.")
        updatePopup.showUpdateButton = false
        updatePopup.buttonsVisible = true
    }
    function checkFailed(message) {
        console.log("QML checkFailed "+message)
        updatePopup.text = qsTr("Version Check failed:\n")+message
        updatePopup.showUpdateButton = false
        updatePopup.buttonsVisible = true
    }
    Connections {
        target: dataUtil
        onUpdateAvailable: updateAvailable(apiVersion)
    }
    Connections {
        target: dataUtil
        onNoUpdateRequired: noUpdateRequired()
    }
    Connections {
        target: dataUtil
        onCheckForUpdateFailed: checkFailed(message)
    }

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
        conference = dataManager.conferencePropertyList[0]
        console.log("Init done from Home Page")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from Home Page")
    }
} // flickable
