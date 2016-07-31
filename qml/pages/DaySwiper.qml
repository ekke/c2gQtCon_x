// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../common"
import "../tabs"

Page {
    id: navSwipePage
    // index to get access to Loader (Destination)
    property int myIndex: index
    property string name: "dayNavPage"

    // because of https://bugreports.qt.io/browse/QTBUG-54260
    // lastCurrentIndex will remember currentIndex, so we can reset before Page becomes currentItem on StackView
    property int lastCurrentIndex: 0

    property alias currentIndex: navSwipePane.currentIndex

    property bool tabBarIsFixed: true
    property var tabButtonModel: [{"name": "Thursday"},
        {"name": "Friday"},
        {"name": "Saturday"},
        {"name": "Sunday"}
    ]

    header: isLandscape? null : tabBar

    Loader {
        id: tabBar
        visible: !isLandscape
        active: !isLandscape
        source: "../tabs/TextTabBar.qml"
        onLoaded: {
            console.log("Tab Bar LOADED")
            if(item) {
                item.currentIndex = navSwipePane.currentIndex
            }
        }
    }
    Loader {
        id: tabBarFloating
        visible: isLandscape
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        active: isLandscape
        source: "../tabs/TextTabBar.qml"
        onLoaded: {
            console.log("Floating Tab Bar LOADED")
            if(item) {
                item.currentIndex = navSwipePane.currentIndex
            }
        }
    }

    SwipeView {
        id: navSwipePane
        focus: true
        anchors.top: isLandscape? tabBarFloating.bottom : parent.top
        anchors.topMargin: isLandscape? 6 : 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: 0
        // currentIndex is the NEXT index swiped to
        onCurrentIndexChanged: {
            console.log("Day Swipe View current index changed: "+currentIndex)
            if(isLandscape) {
                tabBarFloating.item.currentIndex = currentIndex
            } else {
                tabBar.item.currentIndex = currentIndex
            }
        }

        function goToPage(pageIndex) {
            if(pageIndex == navSwipePane.currentIndex) {
                // it's the current page
                return
            }
            if(pageIndex > 3 || pageIndex < 0) {
                return
            }
            navSwipePane.currentIndex = pageIndex
        } // goToPage

        // only 4 Pages - all preloaded
        Repeater {
            model: 4

            Loader {
                id: pageOneLoader
                active: true
                source: "DayPage.qml"
                onLoaded: {
                    item.weekday = index+4
                    item.init()
                }
            }

        } // day repeater




    } // navSwipePane

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT day navSwipePage")
    }
    function cleanup() {
        console.log("CLEANUP navSwipePage")
    }

} // navSwipePage
