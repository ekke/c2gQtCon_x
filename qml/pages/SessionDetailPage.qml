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
    property int sessionId: -2
    onSessionIdChanged: {
        if(sessionId > 0) {
            session = dataManager.findSessionBySessionId(sessionId)
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            // customer = order.customerAsDataObject
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
                        anchors.verticalCenter: parent.verticalCenter
                        text: sessionDetailPage.characterForButton()
                    } // button one char
                    LabelSubheading {
                        Layout.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        text: sessionDetailPage.textForSessionType()
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
                    IconActive{
                        imageSize: 24
                        imageName: "directions.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: qsTr("Room: ") + session.roomAsDataObject.roomName
                    }
                }

                HorizontalDivider{}
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

            }

        }// root pane

    } // flickable

    function characterForButton() {
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
    function textForSessionType() {
        var s
        if(session.isTraining) {
            s = qsTr("Training ")
        } else
        if(session.isLightning) {
            s = qsTr("Lightning Talk")
        } else
        if(session.isKeynote) {
            s = qsTr("Keynote")
        } else
        if(session.isCommunity) {
            s = qsTr("Community")
        } else {
            s =qsTr("Session")
        }

        return s + " (" + session.minutes + qsTr(" Minutes)")
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SessionDetailPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SessionDetailPage"))
    }

} // page
