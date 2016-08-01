// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: dayListPage
    focus: true
    property string name: "dayListPage"
    property Day conferenceDay
    bottomPadding: 24
    topPadding: 16

    // SECTION HEADER
    Component {
        id: sectionHeading
        ColumnLayout {
            width: parent.width
            RowLayout {
                Layout.topMargin: 6
                spacing: 20
                IconColored {
                    Layout.leftMargin: 16
                    imageSize: 36
                    imageName: "time.png"
                }
                LabelTitle {
                    text: section.substr(section.length - 5) // TODO .toLocaleTimeString("HH:mm")
                    anchors.verticalCenter: parent.verticalCenter
                    color: primaryColor
                    font.bold: true
                }
            } // row layout
            HorizontalListDivider{}
        } // col layout
    }


    // LIST ROW DELEGTE
    Component {
        id: sessionRowComponent
        ColumnLayout {
            id: sessionRow
            // without this divider not over total width
            implicitWidth: appWindow.width
            RowLayout {
                spacing: 20
                Layout.leftMargin: 16+12
                Layout.rightMargin: 6
                Layout.topMargin: 6
                ColumnLayout {
                    CharCircle {
                        size: 24
                        text: sessionRow.ListView.view.characterForButton(model.modelData)
                    }
                } // left column
                ColumnLayout {
                    Layout.fillWidth: true
                    // without setting a maximum width, word wrap not working
                    Layout.maximumWidth: appWindow.width-150
                    Layout.minimumWidth: appWindow.width-150
                    spacing: 0
                    LabelSubheading {
                        rightPadding: 12
                        text: model.modelData.title
                        font.bold: true
                        wrapMode: Label.WordWrap
                        maximumLineCount: 2
                        elide: Label.ElideRight
                    } // label

                    LabelBody {
                        visible: model.modelData.subtitle.length
                        rightPadding: 12
                        text: model.modelData.subtitle
                        wrapMode: Label.WordWrap
                        maximumLineCount: 2
                        elide: Label.ElideRight
                    }
                } // middle column
                ListRowButton {
                    onClicked: {
                        navPane.pushSessionDetail(model.modelData.sessionId)
                    }
                }
                ColumnLayout {
                    Layout.rightMargin: 8
                    IconActive {
                        transform: Translate { x: -36 }
                        imageSize: 48
                        imageName: "stars.png"
                        opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                        ListRowButton {
                            onClicked: {
                                model.modelData.isFavorite = !model.modelData.isFavorite
                            }
                        }
                    } // favoritesIcon
                } // right column
            } // end Row Layout
            HorizontalListDivider{}
        } // end Col Layout speaker row
    } // sessionRowComponent

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
        // QList<Session*>
        //model: dataManager.sessionPropertyList

        delegate: sessionRowComponent

        section.property: "sortKey"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading

        ScrollIndicator.vertical: ScrollIndicator { }

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
    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded

    function init() {
        console.log(qsTr("Init done from dayListPage"))
        console.log("Day# "+dataManager.dayPropertyList.length)
        conferenceDay = dataManager.dayPropertyList[index]
        console.log(conferenceDay.conferenceDay)
        console.log("Sessions:"+conferenceDay.sessionsPropertyList.length)
        listView.model = conferenceDay.sessionsPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from dayListPage"))
    }
} // end primaryPage
