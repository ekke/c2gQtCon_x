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

                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 6
                    Layout.bottomMargin: 12
                    SpeakerImageItem {
                        id: speakerImage
                        anchors.top: parent.top
                    }
                    ColumnLayout {
                        LabelHeadline {
                            leftPadding: 10+6
                            text: speaker.name.length? speaker.name : qsTr("Unnamed Speaker")
                            color: accentColor
                        }

                        LabelSubheading {
                            leftPadding: 10+6
                            rightPadding: 10
                            wrapMode: Text.WordWrap
                            text: speaker.bio
                        }
                    }
                } // row
                LabelBodySecondary {
                    text: "id "+speaker.speakerId
                    font.italic: true
                    transform: Translate{y: -12}
                }
                HorizontalDivider{
                    height: 3
                    transform: Translate{y: -8}
                }
                RowLayout {
                    visible: speaker.sessionsPropertyList.length
                    Layout.leftMargin: 16
                    IconActive {
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 36
                        imageName: "schedule.png"
                    } // scheduleItemImage
                    LabelHeadline {
                        leftPadding: 10
                        text: qsTr("Talks")
                        color: primaryColor
                    }
                }
                LabelBodySecondary {
                    visible: speaker.sessionsPropertyList.length
                    leftPadding: 16
                    font.italic: true
                    text: qsTr("Tap on the Talk Icon or Menu Button to get the Details.\nTap on the Star Icon to add to / remove from your personal schedule.")
                    wrapMode: Text.WordWrap
                }
                HorizontalListDivider{
                    visible: speaker.sessionsPropertyList.length
                }
                // S E S S I O N    Repeater
                Repeater {
                    id: sessionRepeater
                    model: speaker.sessionsPropertyList
                    property int sessionRepeaterIndex: index

                    Pane {
                        topPadding: 4
                        leftPadding: 0
                        rightPadding: 0
                        Layout.fillWidth: true

                        // dirty hack with some Buttons to get click events
                        // without loosing click from Menu or Favorites
                        ColumnLayout {
                            id: sessionRow
                            Layout.fillWidth: true
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
                                    CharCircle {
                                        Layout.leftMargin: 14
                                        size: 36
                                        text: dataUtil.letterForButton(modelData)
                                    }
                                    LabelBody {
                                        text: modelData.minutes + qsTr(" Minutes")
                                    }
                                    ListRowButton {
                                        onClicked: {
                                            navPane.pushSessionDetail(modelData.sessionId)
                                        }
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
                                            wrapMode: Text.WordWrap
                                        }
                                        IconActive {
                                            //id: menuIcon
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
                                                    visible: modelData.roomAsDataObject.inAssets
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
                                            transform: Translate { x: 6; y: 8 }
                                            imageSize: 36
                                            imageName: "stars.png"
                                            opacity: modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    modelData.isFavorite = !modelData.isFavorite
                                                    if(modelData.isFavorite) {
                                                        appWindow.showToast(qsTr("Added to Personal Schedule"))
                                                    } else {
                                                        appWindow.showToast(qsTr("Removed from Personal Schedule"))
                                                    }
                                                    if(appWindow.myScheduleActive) {
                                                        dataUtil.refreshMySchedule()
                                                    }
                                                }
                                            }
                                        } // favoritesIcon
                                    } // repeater time and room besides favorite button

                                    RowLayout {
                                        // repeater track row
                                        visible: trackLabel.text.length
                                        IconActive{
                                            imageSize: 18
                                            imageName: "tag.png"
                                        }
                                        Rectangle {
                                            width: 16
                                            height: 16
                                            color: dataUtil.trackColor(modelData.sessionTrack)
                                            radius: width / 2
                                        }
                                        LabelBody {
                                            id: trackLabel
                                            text: modelData.sessionTrackAsDataObject.name != "*****" ? modelData.sessionTrackAsDataObject.name : ""
                                            rightPadding: 16
                                            wrapMode: Text.WordWrap
                                        }
                                    } // repeater track row

                                    LabelSubheading {
                                        text: modelData.title
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                        ListRowButton {
                                            onClicked: {
                                                navPane.pushSessionDetail(modelData.sessionId)
                                            }
                                        }
                                    } // title
                                    LabelBody {
                                        visible: modelData.subtitle.length
                                        text: modelData.subtitle
                                        wrapMode: Label.WordWrap
                                        ListRowButton {
                                            onClicked: {
                                                navPane.pushSessionDetail(modelData.sessionId)
                                            }
                                        }
                                    } // subtitle
                                    RowLayout {
                                        id: presenterRow
                                        visible: modelData.presenterPropertyList.length > 1
                                        property var presenter: modelData.presenterPropertyList
                                        // speakers row
                                        IconActive{
                                            Layout.alignment: Qt.AlignTop
                                            imageSize: 18
                                            imageName: "speaker.png"
                                        }
                                        ColumnLayout {
                                            Repeater {
                                                id: presenterRepeater
                                                model: presenterRow.presenter
                                                LabelBody {
                                                    text: modelData.name
                                                }
                                            } // presenterRepeater
                                        } // presenter Column
                                    } // repeater track row

                                } // // repeater right column
                            } // repeater base row
                            HorizontalListDivider{}
                        } // repeater sessionRow
                    } // session pane
                } // session repeater
            } // col layout
        } // root

        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable
    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from SpeakerDetailPage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from SpeakerDetailPage")
    }

} // speakerDetailPage
