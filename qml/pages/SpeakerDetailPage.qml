// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"
import "../navigation"

Page {
    id: speakerDetailPage
    focus: true
    property string name: "SpeakerDetailPage"

    property Speaker speaker
    property int speakerId: -2
    onSpeakerIdChanged: {
        if(speakerId > 0) {
            speaker = dataManager.findSpeakerBySpeakerId(speakerId)
            speakerImage.speaker = speaker
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            // customer = order.customerAsDataObject
        }
    }


    Flickable {
        id: flickable
        property string name: "speakerDetail"
        // need some extra space if scrolling to bottom
        // and nothing covered by the FAB
        contentHeight: root.implicitHeight + 60
        anchors.fill: parent

        Pane {
            id: root
            anchors.fill: parent
            ColumnLayout {
                Layout.fillWidth: true
                anchors.right: parent.right
                anchors.left: parent.left
                LabelHeadline {
                    topPadding: 16
                    bottomPadding: 16
                    leftPadding: 10
                    text: speaker.name.length? speaker.name : qsTr("Unnamed Speaker")
                }
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 6
                    Layout.bottomMargin: 12
                    SpeakerImageItem {
                        id: speakerImage
                        anchors.top: parent.top
                    }
                    LabelSubheading {
                        leftPadding: 10+6
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: speaker.bio
                        //Layout.preferredWidth: 2
                    }
                } // row
                HorizontalDivider {}
                LabelHeadline {
                    visible: speaker.sessionsPropertyList.length
                    leftPadding: 10
                    text: qsTr("Talks")
                    color: primaryColor
                }
                LabelBodySecondary {
                    visible: speaker.sessionsPropertyList.length
                    leftPadding: 10
                    font.italic: true
                    text: qsTr("Tap on the Talk Icon or Menu Button to get the Details.\nTap on the Star Icon to add to / remove from your personal schedule.")
                    wrapMode: Text.WordWrap
                }
                HorizontalListDivider{
                    visible: speaker.sessionsPropertyList.length
                }
                // S P E A K E R    Repeater
                Repeater {
                    model: speaker.sessionsPropertyList

                    Pane {
                        topPadding: 4
                        leftPadding: 0
                        rightPadding: 0
                        Layout.fillWidth: true
                        ColumnLayout {
                            // base column
                            anchors.left: parent.left
                            anchors.right: parent.right
                            RowLayout {
                                // base row
                                Layout.leftMargin: 16
                                Layout.rightMargin: 6
                                Layout.bottomMargin: 2
                                ColumnLayout {
                                    // repeater left column
                                    Layout.maximumWidth: speakerImage.width
                                    Layout.minimumWidth: speakerImage.width
                                    Layout.rightMargin: 6
                                    anchors.top: parent.top
                                    ButtonOneChar {
                                        Layout.leftMargin: 14
                                        Layout.rightMargin: 14
                                        text: speakerDetailPage.characterForButton(modelData)
                                        backgroundColor: accentColor
                                        textColor: textOnAccent
                                        onClicked: {
                                            navPane.pushSessionDetail(modelData.sessionId)
                                        }
                                    }
                                    LabelBody {
                                        text: modelData.minutes + qsTr(" Minutes")
                                    }
                                } // repeater left column

                                ColumnLayout {
                                    // repeater right column
                                    Layout.fillWidth: true
                                    Layout.leftMargin: 10
                                    Layout.rightMargin: 10
                                    RowLayout {
                                        // repeater date row
                                        IconActive{
                                            imageSize: 18
                                            imageName: "calendar.png"
                                        }
                                        LabelBody {
                                            text: modelData.sessionDayAsDataObject.conferenceDay.toLocaleDateString()
                                        }
                                        IconActive {
                                            //id: menuIcon
                                            transform: Translate { y: -8 }
                                            imageSize: 24
                                            imageName: "more_vert.png"
                                            anchors.right: parent.right
                                            // anchors.top: parent.top
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    optionsMenu.open()
                                                }
                                            } // mouse area
                                            Menu {
                                                id: optionsMenu
                                                x: parent.width - width
                                                transformOrigin: Menu.TopRight
                                                MenuItem {
                                                    text: qsTr("Session Details")
                                                    onTriggered: {
                                                        navPane.pushSessionDetail(modelData.sessionId)
                                                    }
                                                }
                                                MenuItem {
                                                    text: qsTr("Room Info")
                                                    onTriggered: {
                                                        navPane.pushRoomDetail(modelData.roomAsDataObject.roomId)
                                                    }
                                                }
                                            } // end optionsMenu
                                        } // menuIcon
                                    } // // repeater date row
                                    RowLayout {
                                        // repeater time and room besides favorite button
                                        ColumnLayout {
                                            // time room column
                                            Layout.fillWidth: true
                                            RowLayout {
                                                // repeater time row
                                                IconActive{
                                                    imageSize: 18
                                                    imageName: "time.png"
                                                }
                                                LabelBody {
                                                    text: modelData.startTime.toLocaleTimeString("HH:mm") + " - " + modelData.endTime.toLocaleTimeString("HH:mm")
                                                }
                                            } // repeater time row
                                            RowLayout {
                                                // repeater room row
                                                IconActive{
                                                    imageSize: 18
                                                    imageName: "directions.png"
                                                }
                                                LabelBody {
                                                    text: modelData.roomAsDataObject.roomName
                                                }
                                            } // repeater room row
                                        } // // time room column
                                        IconActive {
                                            transform: Translate { y: 8 }
                                            imageSize: 36
                                            imageName: "stars.png"
                                            opacity: modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    modelData.isFavorite = !modelData.isFavorite
                                                }
                                            }
                                        } // favoritesIcon
                                    } // repeater time and room besides favorite button

                                    RowLayout {
                                        // repeater track row
                                        visible: trackLabel.text.length
                                        IconActive{
                                            imageSize: 18
                                            imageName: "description.png"
                                        }
                                        LabelBody {
                                            id: trackLabel
                                            text: modelData.sessionTrackAsDataObject.name != "*****" ? modelData.sessionTrackAsDataObject.name : ""
                                        }
                                    } // repeater track row
                                    LabelSubheading {
                                        text: modelData.title
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                    } // title
                                    LabelBody {
                                        visible: modelData.subtitle.length
                                        text: modelData.subtitle
                                        wrapMode: Label.WordWrap
                                    } // subtitle
                                    LabelSubheading {
                                        visible: modelData.presenterPropertyList.length > 1
                                        text: qsTr("Co - Speaker:")
                                        color: primaryColor
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                    } // co-speakers header
                                    LabelBody {
                                        visible: modelData.presenterPropertyList.length > 1
                                        text: speakerDetailPage.coSpeakers(modelData)
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                    } // co-speakers names
                                } // // repeater right column
                            } // repeater base row
                            HorizontalListDivider{}
                        } // rep base col
                    } // speaker pane
                } // repeater
            } // col layout
        } // root

        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable
    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    function characterForButton(session) {
        if(session.isTraining) {
            return "T"
        }
        if(session.isLightning) {
            return "L"
        }
        if(session.isKeynote) {
            return "K"
        }
        if(session.isCommunity) {
            return "C"
        }
        return "S"
    }
    function coSpeakers(session) {
        var s = ""
        for (var i = 0; i < session.presenterPropertyList.length; i++) {
            if(session.presenterPropertyList[i].speakerId != speakerDetailPage.speakerId) {
                if(s.length) {
                    s += "\n"
                }
                s += session.presenterPropertyList[i].name
            }
        }
        return s
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SpeakerDetailPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SpeakerDetailPage"))
    }

} // speakerDetailPage
