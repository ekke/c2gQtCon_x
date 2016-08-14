// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: roomListPage
    focus: true
    property string name: "RoomListPage"
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
        // QList<Room*>
        //model: dataManager.roomPropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: roomLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: sessionsPropertyList.length? roomRowComponent :emptyRoomComponent

            Component {
                id: emptyRoomComponent
                Item {}
            }

            // LIST ROW DELEGTE
            Component {
                id: roomRowComponent
                ColumnLayout {
                    id: roomRow
                    visible: model.modelData.sessionsPropertyList.length
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
                            Layout.maximumWidth: appWindow.width-60-64
                            Layout.minimumWidth: appWindow.width-60-64

                            LabelHeadline {
                                text: qsTr("Room ") + model.modelData.roomName
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
                                navPane.pushRoomSessions(model.modelData.roomId)
                            }
                        }
                        ColumnLayout {
                            Layout.maximumWidth: 64
                            Layout.minimumWidth: 64
                            Item {
                                visible: model.modelData.inAssets
                                width: 64
                                height: 64
                                Image {
                                    id: roomImage
                                    width: 64
                                    height: 64
                                    fillMode: Image.PreserveAspectFit
                                    source: model.modelData.inAssets? "qrc:/data-assets/conference/floorplan/room_"+model.modelData.roomId+".png" : ""
                                    horizontalAlignment: Image.AlignLeft
                                    verticalAlignment: Image.AlignTop
                                    transform: Translate {x: -16 }
                                } // image
                                ListRowButton {
                                    onClicked: {
                                        navPane.pushRoomDetail(model.modelData.roomId)
                                    }
                                }
                            } // item
                        } // col room image

                    }

                    HorizontalListDivider{}
                } // end Col Layout speaker row
            } // roomRowComponent

        } // roomLoader

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from RoomListPage"))
        console.log("Tracks # "+dataManager.roomPropertyList.length)
        dataUtil.resolveSessionsForRooms()
        listView.model = dataManager.roomPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from RoomListPage"))
    }
} // end RoomListPage
