// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: myScheduleListPage
    focus: true
    property string name: "myScheduleListPage"
    property SessionLists sessionLists
    bottomPadding: 6
    topPadding: 6


    // SECTION HEADER DELEGATE
    Component {
        id: sectionHeading
        Pane {
            topPadding: 0
            bottomPadding: 12
            leftPadding: 0
            rightPadding: 0
            width: parent.width
            background: Rectangle{color: Material.listHighlightColor}
            ColumnLayout {
                y: -6
                width: parent.width
                height: 48
                RowLayout {
                    Layout.topMargin: 6
                    spacing: 10
                    // TODO BUG IconColored sometimes washed out in list
                    IconActive {
                        Layout.leftMargin: 16 +36 + 20
                        imageName: "time.png"
                    }
                    LabelTitle {
                        text: dataUtil.localWeekdayAndTime(section)
                        anchors.verticalCenter: parent.verticalCenter
                        color: primaryColor
                        font.bold: true
                    }
                } // section row
                //HorizontalListDivider{}
            } // section col
        } // section Pane
    } // sectionHeading Component

    // LIST VIEW
    SessionListView {
        id: listView
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded

    function init() {
        console.log(qsTr("Init done from myScheduleListPage"))
        sessionLists = dataUtil.mySchedule()

        console.log(" MySchedule Sessions:"+sessionLists.scheduledSessionsPropertyList.length)
        listView.model = sessionLists.scheduledSessionsPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from myScheduleListPage"))
    }
} // end primaryPage
