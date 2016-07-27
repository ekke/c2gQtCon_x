// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: speakerListPage
    focus: true
    property string name: "SpeakerListPage"
    bottomPadding: 24
    topPadding: 16

    // SECTION HEADER
    Component {
        id: sectionHeading
        ToolBar {
            width: parent.width
            // using z 1 because section header must under list header
            z:1
            background: Rectangle{color: Material.background}
            ColumnLayout {
                width: parent.width
                LabelHeadline {
                    topPadding: 6
                    bottomPadding: 6
                    leftPadding: 24
                    text: section
                    anchors.verticalCenter: parent.verticalCenter
                    color: primaryColor
                }
                HorizontalListDivider{}
            } // col layout
        } // toolbar
    }

    // LIST ROW DELEGTE
    Component {
        id: speakerRowComponent
        ColumnLayout {
            id: dataColumn
            Layout.fillWidth: true
            anchors.left: parent.left
            anchors.right: parent.right
            RowLayout {
                spacing: 20
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.topMargin: 6
                Item {
                    id: imageItem
                    height: 64
                    width: 64
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.Pad
                        source: model.modelData.hasSpeakerImage()? "qrc:/data-assets/conference/speakerImages/speaker_"+model.modelData.speakerId+"."+model.modelData.speakerImageAsDataObject.suffix : ""
                    } // image
                } // icon item

                LabelTitle {
                    topPadding: 6
                    leftPadding: 24
                    rightPadding: 12
                    text: model.modelData.name.length? model.modelData.name : qsTr("Unnamed Speaker")
                    wrapMode: Label.WordWrap
                } // label
            } // end Row Layout
            HorizontalListDivider{}
        } // end Col Layout
    } // speakerRowComponent

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
        // QList<Speaker*>
        //model: dataManager.speakerPropertyList

        delegate: speakerRowComponent
        // header: headerComponent
        // in Landscape header scrolls away
        // in protrait header always visible
        // headerPositioning: isLandscape? ListView.PullBackHeader : ListView.OverlayHeader

        section.property: "sortGroup"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from SpeakerListPage"))
        console.log("Speaker# "+dataManager.speakerPropertyList.length)
        listView.model = dataManager.speakerPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from SpeakerListPage"))
    }
} // end primaryPage
