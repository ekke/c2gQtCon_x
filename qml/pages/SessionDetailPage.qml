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
                LabelHeadline {
                    topPadding: 16
                    bottomPadding: 16
                    leftPadding: 10
                    text: session.title
                    wrapMode: Text.WordWrap
                }



            }

        }// root pane

    } // flickable

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SessionDetailPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SessionDetailPage"))
    }

} // page
