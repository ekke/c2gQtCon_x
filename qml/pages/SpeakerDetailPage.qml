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

    // property Customer customer
    property Speaker speaker
    property SettingsData settingsData
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

        contentHeight: root.implicitHeight
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
                    Layout.fillWidth: true
                    Layout.leftMargin: 16
                    Layout.rightMargin: 6
                    Layout.bottomMargin: 12
                    SpeakerImageItem {
                        id: speakerImage
                        anchors.top: parent.top
                    }
                    LabelSubheading {
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: speaker.bio
                        //Layout.preferredWidth: 2
                    }
                } // row
                HorizontalDivider {}
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("Talks")
                    color: primaryColor
                }
                HorizontalDivider{
                    visible: speaker.sessionsPropertyList.length
                }

                Repeater {
                    model: speaker.sessionsPropertyList

                    ColumnLayout {
                        Layout.fillWidth: true
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 16
                            Layout.rightMargin: 6

                            ColumnLayout {
                                Layout.fillWidth: true
                                anchors.top: parent.top
                                ButtonOneChar {
                                    Layout.leftMargin: 14
                                    Layout.rightMargin: 14
                                    text: speakerDetailPage.characterForButton(modelData)
                                    backgroundColor: accentColor
                                    textColor: textOnAccent
                                }
                                LabelBody {
                                    text: modelData.minutes + qsTr(" Minutes")
                                }


                            }



                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10

                                RowLayout {
                                    Layout.fillWidth: true
                                    IconActive{
                                        imageSize: 18
                                        imageName: "calendar.png"
                                    }
                                    LabelBody {
                                        text: modelData.sessionDayAsDataObject.conferenceDay.toLocaleDateString()
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    IconActive{
                                        imageSize: 18
                                        imageName: "time.png"
                                    }
                                    LabelBody {
                                        text: modelData.startTime.toLocaleTimeString("HH:mm") + " - " + modelData.endTime.toLocaleTimeString("HH:mm")
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    IconActive{
                                        imageSize: 18
                                        imageName: "directions.png"
                                    }
                                    LabelBody {
                                        text: modelData.roomAsDataObject.roomName
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    visible: trackLabel.text.length
                                    IconActive{
                                        imageSize: 18
                                        imageName: "description.png"
                                    }
                                    LabelBody {
                                        id: trackLabel
                                        text: modelData.sessionTrackAsDataObject.name != "*****" ? modelData.sessionTrackAsDataObject.name : ""
                                    }
                                }

                                LabelSubheading {
                                    text: modelData.title
                                    font.bold: true
                                    wrapMode: Label.WordWrap
                                }

                                LabelBody {
                                    visible: modelData.subtitle.length
                                    text: modelData.subtitle
                                    wrapMode: Label.WordWrap
                                }

                                LabelSubheading {
                                    visible: modelData.presenterPropertyList.length > 1
                                    text: qsTr("Co - Speaker:")
                                    color: primaryColor
                                    font.bold: true
                                    wrapMode: Label.WordWrap
                                }
                                LabelBody {
                                    visible: modelData.presenterPropertyList.length > 1
                                    text: speakerDetailPage.coSpeakers(modelData)
                                    font.bold: true
                                    wrapMode: Label.WordWrap
                                }

                            } // col in row
                        } // row
                        HorizontalListDivider{}
                    } // rep col
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

} // carPage
