// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

ListView {
    id: listView
    focus: true
    clip: true
    // highlight: Rectangle {color: Material.listHighlightColor }
    currentIndex: -1
    anchors.fill: parent
    // setting the margin to be able to scroll the list above the FAB to use the Switch on last row
    bottomMargin: 40
    // QList<Session*>
    //model: dataManager.sessionPropertyList
    // or
    // sessionLists.scheduledSessionsPropertyList

    delegate:

        Loader {
        id: sessionLoader
        // define Components inside Loader to enable direct access to ListView functions and modelData
        sourceComponent: hasScheduleItem()? scheduleRowComponent : sessionRowComponent

        // LIST ROW DELEGATES
        Component {
            id: scheduleRowComponent

            ColumnLayout {
                id: scheduleRow
                // without this divider not over total width
                implicitWidth: appWindow.width
                RowLayout {
                    spacing: 20
                    Layout.leftMargin: 20
                    Layout.rightMargin: 6
                    Layout.topMargin: 6
                    ColumnLayout {
                        IconActive {
                            //transform: Translate { x: -36 }
                            imageSize: 36
                            imageName: listView.scheduleItemImage(model.modelData)
                            // opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                        } // scheduleItemImage
                    } // left column
                    ColumnLayout {
                        Layout.fillWidth: true
                        // without setting a maximum width, word wrap not working
                        Layout.maximumWidth: appWindow.width-150
                        Layout.minimumWidth: appWindow.width-150
                        spacing: 0
                        LabelTitle {
                            rightPadding: 12
                            text: model.modelData.title
                            //font.bold: true
                            wrapMode: Label.WordWrap
                            maximumLineCount: 2
                            elide: Label.ElideRight
                        } // label
                        LabelSubheading {
                            rightPadding: 12
                            bottomPadding: 6
                            font.italic: true
                            text: model.modelData.startTime.toLocaleTimeString("HH:mm") + " - " + model.modelData.endTime.toLocaleTimeString("HH:mm")
                        }
                    } // middle column
                    ListRowButton {
                        onClicked: {
                            navPane.pushSessionDetail(model.modelData.sessionId)
                        }
                    }
                }
            } // scheduleRow

        } // scheduleRowComponent

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
                            text: listView.characterForButton(model.modelData)
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
                        RowLayout {
                            LabelBody {
                                Layout.fillWidth: false
                                text: model.modelData.startTime.toLocaleTimeString("HH:mm") + " - " + model.modelData.endTime.toLocaleTimeString("HH:mm") + ","
                            }
                            IconActive{
                                imageSize: 18
                                imageName: "directions.png"
                            }
                            LabelBody {
                                Layout.fillWidth: false
                                text: qsTr("Room ") + model.modelData.roomAsDataObject.roomName
                            }
                        }
                        RowLayout {
                            visible: speakerNamesLabel.text.length
                            IconActive{
                                imageSize: 18
                                imageName: "speaker.png"
                            }
                            LabelBody {
                                id: speakerNamesLabel
                                font.italic: true
                                text: listView.speakerNames(model.modelData)
                                elide: Label.ElideRight
                            }
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
                                    if(appWindow.myScheduleActive) {
                                        dataUtil.refreshMySchedule()
                                    }
                                }
                            }
                        } // favoritesIcon
                    } // right column
                } // end Row Layout
                HorizontalListDivider{}
            } // end Col Layout speaker row
        } // sessionRowComponent

    }

    section.property: "sortKey"
    section.criteria: ViewSection.FullString
    section.delegate: sectionHeading

    ScrollIndicator.vertical: ScrollIndicator { }
    function speakerNames(session) {
        var s = ""

        for (var i = 0; i < session.presenterPropertyList.length; i++) {
            var pName = session.presenterPropertyList[i].name
            if(pName.length) {
                if(s.length) {
                    s += ", "
                }
                s += pName
            }
        } // for
        return s
    }

    function scheduleItemImage(session) {
        if(session.scheduleItemAsDataObject.isRegistration) {
            return "key.png"
        }
        if(session.scheduleItemAsDataObject.isLunch) {
            return "lunch.png"
        }
        if(session.scheduleItemAsDataObject.isEvent) {
            return "party_event.png"
        }
        return "break.png"
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
        if(session.isMeeting) {
            return "M"
        }
        if(session.isUnconference) {
            return "U"
        }
        return "S"
    }
} // end listView
