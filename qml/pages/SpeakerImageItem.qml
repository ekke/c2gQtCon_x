// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"

Item {
    property Speaker speaker
    property string assetsSource: "qrc:/data-assets/conference/speakerImages/speaker_"
    property string dataSource: dataUtil.conferenceDataPath4QML()+"speakerImages/speaker_"
    id: imageItem
    height: 64
    width: 64
    Image {
        visible: speaker && speaker.hasSpeakerImage()
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.Pad
        source: speaker.hasSpeakerImage()? speaker.speakerImageAsDataObject.inAssets? assetsSource+speaker.speakerId+"."+speaker.speakerImageAsDataObject.suffix
                                           : speaker.speakerImageAsDataObject.inData? dataSource+speaker.speakerId+"."+speaker.speakerImageAsDataObject.suffix : "" : ""
    } // image
    IconInactive {
        visible: speaker && !speaker.hasSpeakerImage()
        imageSize: 48
        imageName: "person.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.Pad
    }
} // icon item
