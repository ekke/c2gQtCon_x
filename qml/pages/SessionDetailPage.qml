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
    id: sessionDetailPage
    focus: true
    property string name: "SessionDetailPage"

    property Session session
    property bool isScheduleItem: false
    property int sessionId: -2
    onSessionIdChanged: {
        if(sessionId > 0) {
            session = dataManager.findSessionBySessionId(sessionId)
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            // customer = order.customerAsDataObject
            isScheduleItem = session.hasScheduleItem()
        }
    }

    Flickable {
        id: flickable
        property string name: "sessionDetail"
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
                    Layout.rightMargin: 16
                    Layout.bottomMargin: 12
                    LabelTitle {
                        text: session.title
                        wrapMode: Text.WordWrap
                    }
                }
                HorizontalDivider{}
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    ButtonOneCharUncolored {
                        visible: !isScheduleItem
                        anchors.verticalCenter: parent.verticalCenter
                        text: dataUtil.letterForButton(session)
                    } // button one char
                    IconActive {
                        visible: isScheduleItem
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 24
                        imageName: isScheduleItem? dataUtil.scheduleItemImageForSession(session) : ""
                    } // scheduleItemImage

                    LabelSubheading {
                        Layout.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        text: dataUtil.textForSessionType(session)
                        wrapMode: Text.WordWrap
                    }
                    IconActive {
                        visible: !isScheduleItem
                        imageSize: 36
                        imageName: "stars.png"
                        opacity: session.isFavorite? opacityToggleActive : opacityToggleInactive
                        anchors.right: parent.right
                        anchors.top: parent.top
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                session.isFavorite = !session.isFavorite
                                if(session.isFavorite) {
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
                }
                RowLayout {
                    visible: trackLabel.text.length > 0
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "tag.png"
                    }
                    Rectangle {
                        Layout.leftMargin: 16
                        width: 16
                        height: 16
                        color: dataUtil.trackColor(session.sessionTrack)
                        radius: width / 2
                    }
                    LabelSubheading {
                        id: trackLabel
                        rightPadding: 16
                        text: dataUtil.textForSessionTrack(session)
                        wrapMode: Text.WordWrap
                    }
                }
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "calendar.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: session.sessionDayAsDataObject.conferenceDay.toLocaleDateString()
                    }
                }
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "time.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: session.startTime.toLocaleTimeString("HH:mm") + " - " + session.endTime.toLocaleTimeString("HH:mm")
                    }
                }
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    transform: Translate{y: -10}
                    IconActive{
                        imageSize: 24
                        imageName: "directions.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: qsTr("Room: ") + session.roomAsDataObject.roomName
                    }
                    FloatingActionMiniButton {
                        z: 1
                        visible: session.roomAsDataObject.inAssets
                        transform: Translate{y: -6}
                        showShadow: true
                        imageSource: "qrc:/images/"+iconOnAccentFolder+"/directions.png"
                        backgroundColor: accentColor
                        anchors.right: parent.right
                        onClicked: {
                            navPane.pushRoomDetail(session.roomAsDataObject.roomId)
                        }
                    } // favoritesIcon
                }
                LabelBodySecondary {
                    text: "id "+session.sessionId
                    font.italic: true
                    transform: Translate{y: -8}
                }
                HorizontalDivider{
                    height: 3
                    transform: Translate{y: -8}
                }
                LabelSubheading {
                    visible: session.subtitle.length
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    text: session.subtitle
                    wrapMode: Text.WordWrap
                }
                LabelSubheading {
                    visible: session.description.length
                    Layout.topMargin: 12
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    text: session.description
                    wrapMode: Text.WordWrap
                }

                HorizontalDivider{
                    visible: session.subtitle.length || session.description.length
                }

                LabelSubheading {
                    visible: session.abstractText.length
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    text: session.abstractText
                    wrapMode: Text.WordWrap
                }

                HorizontalDivider{
                    visible: session.abstractText.length
                }
                RowLayout {
                    visible: session.presenterPropertyList.length
                    Layout.leftMargin: 16
                    IconActive {
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 36
                        imageName: "speaker.png"
                    } // speaker image
                    LabelHeadline {
                        leftPadding: 10
                        text: qsTr("Speaker")
                        color: primaryColor
                    }
                }
                LabelBodySecondary {
                    visible: session.presenterPropertyList.length
                    leftPadding: 16
                    font.italic: true
                    text: qsTr("Tap on the Speaker Data to get the Details.")
                    wrapMode: Text.WordWrap
                }
                HorizontalListDivider{
                    visible: session.presenterPropertyList.length
                }

                // S P E A K E R    Repeater
                Repeater {
                    model: session.presenterPropertyList

                    Pane {
                        topPadding: 4
                        leftPadding: 0
                        rightPadding: 0
                        Layout.fillWidth: true

                        ColumnLayout {
                            id: speakerRow
                            // without this divider not over total width
                            implicitWidth: appWindow.width
                            RowLayout {
                                spacing: 20
                                Layout.leftMargin: 16
                                Layout.rightMargin: 6
                                SpeakerImageItem {
                                    speaker: model.modelData
                                }
                                ColumnLayout {
                                    Layout.fillWidth: true
                                    // without setting a maximum width, word wrap not working
                                    Layout.maximumWidth: appWindow.width-120
                                    spacing: 0
                                    LabelSubheading {
                                        rightPadding: 12
                                        text: model.modelData.name.length? model.modelData.name : qsTr("Unnamed Speaker")
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                    } // label
                                    LabelBody {
                                        text: dataUtil.sessionInfoForSpeaker(model.modelData)
                                        rightPadding: 12
                                        wrapMode: Label.WordWrap
                                        maximumLineCount: 3
                                        elide: Label.ElideRight
                                    }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        navPane.pushSpeakerDetail(model.modelData.speakerId)
                                    }
                                } // mouse
                            } // end Row Layout
                            HorizontalListDivider{}
                        } // end Col Layout speaker row

                    }// presenter Pane
                } // speaker repeater

            }

        }// root pane
        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from SessionDetailPage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from SessionDetailPage")
    }

} // page
