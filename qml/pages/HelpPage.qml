// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

import "../common"

Flickable {
    id: flickable
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "Help"

    Pane {
        id: root
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            RowLayout {
                Image {
                    source: "qrc:/images/extra/qtcon-app.png"
                }
                LabelHeadline {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Conference2Go\nQtCon 2016, Berlin\n(Qt 5.7 - Android + iOS)")
                    color: primaryColor
                }
            }
            HorizontalDivider {}
            LabelTitle {
                leftPadding: 10
                text: qsTr("Misc")
                color: accentColor
            }
            HelpRow {
                iconName: "menu.png"
                helpText: qsTr("Menu Button: Opens the Drawer. Drawer can also be opened with Gestures: swiping from left site.")
            }
            HelpRow {
                iconName: "more_vert.png"
                helpText: qsTr("Options Button: Opens a Menu with some options.")
            }
            HelpRow {
                iconName: "arrow_back.png"
                helpText: qsTr("Back Button: Top/Left from TitleBar goes one Page back. On Android you can also use OS - specific Back Button below the Page.")
            }
            HelpRow {
                iconName: "list.png"
                helpText: qsTr("Speed Navigation: Back to the List below in the stack without the need to move fingers to Top/Left Back Button.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("QtCon")
                color: accentColor
            }
            HelpRow {
                iconName: "home.png"
                helpText: qsTr("Homepage - the first Page. From here you can start Updates.")
            }
            HelpRow {
                iconName: "refresh.png"
                helpText: qsTr("QtCon Conference APP already contains the Conference Data, so you can start without waiting for Downloads. To get Schedule- and Speaker Updates tap on the Refresh Button.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Schedule")
                color: accentColor
            }
            HelpRow {
                iconName: "schedule.png"
                helpText: qsTr("Complete Conference Schedule separated by Days and sorted by Starttime. Switch between Conference Days by Swiping left/right or tapping on a Tab from Tab Bar.")
            }
            HelpRow {
                iconName: "stars.png"
                helpText: qsTr("Button to see your Personal Conference Schedule. Mark Sessions for your Personal Schedule by checking the Favorites Button.")
            }
            HelpRow {
                iconName: "goto.png"
                helpText: qsTr("There are many Sessions listed for a Day - to make it easier to jump to a specific Timeslot tap on this Button and select the Time.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Speaker")
                color: accentColor
            }
            HelpRow {
                iconName: "speaker.png"
                helpText: qsTr("List of all Speakers sorted by Last Name.")
            }
            HelpRow {
                iconName: "goto.png"
                helpText: qsTr("To find a specific Speaker tap on this Button and select the Letter.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Tracks")
                color: accentColor
            }
            HelpRow {
                iconName: "tag.png"
                helpText: qsTr("List of all Conference Tracks. Tap on a row to see all Sessions of selected Track.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Venue")
                color: accentColor
            }
            HelpRow {
                iconName: "venue.png"
                helpText: qsTr("Informations and Address of the Venue. From here you can also see a List of all Rooms.")
            }
            HelpRow {
                iconName: "directions.png"
                helpText: qsTr("List of all Rooms. Tap on a Row to see all Sessions running in this Room. Tap on the Thumbnail to see the Floorplan of the selected Room.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Settings")
                color: accentColor
            }
            HelpRow {
                iconName: "settings.png"
                helpText: qsTr("Customize QtCon Conference APP. You can change the Theme (light or dark) and also Primary or Accent Color. Colors can be selected from Material Style Colors.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Talks")
                color: accentColor
            }
            HelpRow {
                iconName: "stars.png"
                helpText: qsTr("The Favorites Button. Tap on it to mark a Session and to add this to your Personal Schedule. Button is a Toggle - tap again to remove from your Personal Schedule.")
            }
            HelpRow {
                iconName: "directions.png"
                helpText: qsTr("See the Floorplan of the Room where the Session runs.")
            }
            HelpRow {
                iconName: "calendar.png"
                helpText: qsTr("Conference Date.")
            }
            HelpRow {
                iconName: "time.png"
                helpText: qsTr("Session Time from - to.")
            }
            HorizontalDivider {}
            LabelTitle {
                leftPadding: 10
                text: qsTr("Navigation")
                color: accentColor
            }
            HelpRow {
                iconName: "my_location.png"
                helpText:  qsTr("QtCon Conference app uses some special Navigation Styles to speed up Navigation.\nIf you don't like this way to go and want to use Google Material Classic Navigation Style you can customize this from 'Settings'.")
            }
            HorizontalDivider {}
            LabelSubheading {
                leftPadding: 10
                rightPadding: 16
                text: qsTr("Need more Infos ?\nFollow @ekkescorner at Twitter\nfollow ekkes blogs:\nhttp://appbus.org and http://ekkes-corner.org")
                font.italic: true
                wrapMode: Text.WordWrap
            }
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from HELP")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from HELP")
    }
} // flickable
