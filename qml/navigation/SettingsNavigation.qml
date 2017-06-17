// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

import "../pages"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "SettingsNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "SettingsNavPane"
        focus: true

        initialItem: SettingsPage{}

        Loader {
            id: primaryColorPageLoader
            active: false
            visible: false
            source: "../pages/PrimaryColorPage.qml"
            onLoaded: {
                navPane.push(item)
            }
        }
        Loader {
            id: accentColorPageLoader
            active: false
            visible: false
            source: "../pages/AccentColorPage.qml"
            onLoaded: {
                navPane.push(item)
            }
        }

        function pushPrimaryColorPage() {
            primaryColorPageLoader.active = true
        }
        function pushAccentColorPage() {
            accentColorPageLoader.active = true
        }
        function popOnePage() {
            var page = pop()
            if(page.name == "PrimaryColorPage") {
                primaryColorPageLoader.active = false
                return
            }
            if(page.name == "AccentColorPage") {
                accentColorPageLoader.active = false
                return
            }
        }

    } // navPane

    // triggered from BACK KEY
    function goBack() {
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT SettingsNavPane")
        navPane.initialItem.init()
    }
    function cleanup() {
        console.log("CLEANUP SettingsNavPane")
    }

} // navPage
