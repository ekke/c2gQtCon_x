// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: trackListPage
    focus: true
    property string name: "trackListPage"
    bottomPadding: 24
    topPadding: 16

    // LIST VIEW
    ListView {
        id: listView
        focus: true
        clip: true
        // highlight: Rectangle {color: Material.listHighlightColor }
        currentIndex: -1
        anchors.fill: parent
        // setting the margin to be able to scroll the list above the FAB to use the Switch on last row
        // bottomMargin: 40
        // QList<SessionTrack*>
        //model: dataManager.sessionTrackPropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: sessionTrackLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: trackRowComponent

            // LIST ROW DELEGTE
            Component {
                id: trackRowComponent
                ColumnLayout {
                    id: trackRow
                    // without this divider not over total width
                    implicitWidth: appWindow.width
                    spacing: 0
                    RowLayout {
                        //implicitWidth: appWindow.width

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 36
                            Layout.rightMargin: 12
                            Layout.topMargin: 12
                            Layout.bottomMargin: 12
                            // without setting a maximum width, word wrap not working
                            Layout.maximumWidth: appWindow.width-60
                            Layout.minimumWidth: appWindow.width-60

                            LabelHeadline {
                                text: model.modelData.name
                                color: primaryColor
                                wrapMode: Label.WordWrap
                            } // label

                            LabelBody {
                                text: model.modelData.sessionsPropertyList.length + qsTr(" Sessions")
                                wrapMode: Label.WordWrap
                                maximumLineCount: 2
                                elide: Label.ElideRight
                            }
                        }
                        ListRowButton {
                            onClicked: {
                                navPane.pushTrackSessions(model.modelData.trackId)
                            }
                        }
                    }

                    HorizontalListDivider{}
                } // end Col Layout speaker row
            } // trackRowComponent

        } // sessionTrackLoader

        ScrollIndicator.vertical: ScrollIndicator { }

        function trackInfo(track) {
            var s = qsTr("No. of Sessions: ")
            s += track.sessionsPropertyList.length
            return s
        }
    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from trackListPage"))
        console.log("Tracks # "+dataManager.sessionTrackPropertyList.length)
        dataUtil.resolveSessionsForTracks()
        listView.model = dataManager.sessionTrackPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from trackListPage"))
    }
} // end tracklistPage
