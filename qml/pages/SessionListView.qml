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
                            imageName: dataUtil.scheduleItemImageForSession(model.modelData)
                            // opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                        } // scheduleItemImage
                    } // left column
                    ColumnLayout {
                        Layout.fillWidth: true
                        // without setting a maximum width, word wrap not working
                        Layout.maximumWidth: appWindow.width-132
                        Layout.minimumWidth: appWindow.width-132
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
            Item {
                id: theItem
                height: sessionRow.height
                implicitWidth: appWindow.width
                Rectangle {
                    anchors.top: theItem.top
                    height: sessionRow.height-2
                    width: 8
                    color: model.modelData.isKeynote? "#B2DFDB" : dataUtil.trackColor(model.modelData.sessionTrack)
                }

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
                                text: dataUtil.letterForButton(model.modelData)
                            }
                        } // left column
                        ColumnLayout {
                            Layout.fillWidth: true
                            // without setting a maximum width, word wrap not working
                            Layout.maximumWidth: appWindow.width-132
                            Layout.minimumWidth: appWindow.width-132
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
                                    text: dataUtil.speakerNamesForSession(model.modelData)
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
                            Layout.rightMargin: 16
                            IconActive {
                                transform: Translate { x: -46 }
                                imageSize: 36
                                imageName: "stars.png"
                                opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                                ListRowButton {
                                    onClicked: {
                                        model.modelData.isFavorite = !model.modelData.isFavorite
                                        if(model.modelData.isFavorite) {
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
                        } // right column
                    } // end Row Layout
                    HorizontalListDivider{}
                } // end Col Layout speaker row

            } // row item



        } // sessionRowComponent

    } // sessionLoader

    section.property: "sortKey"
    section.criteria: ViewSection.FullString
    section.delegate: sectionHeading

    ScrollIndicator.vertical: ScrollIndicator { }

} // end listView
