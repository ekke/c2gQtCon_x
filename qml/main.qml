// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "common"
import "pages"
import "popups"
import "tabs"
import "navigation"

// This app demonstrates HowTo use Qt 5.8 and Qt Quick Controls 2.1, High DPI and more
// This app is NOT a production ready app
// This app's goal is only to help you to understand some concepts
// see blog http://j.mp/qt-x to learn about Qt 5.8 for Material - styled Android or iOS Apps
// learn about this drawer_nav_x app from this article: http://bit.ly/qt-drawer-nav-x
// ekke (Ekkehard gentz) @ekkescorner

ApplicationWindow {
    id: appWindow
    // running on mobiles you don't need width and height
    // ApplicationWindow will always fill entire screen
    // testing also on desktop it makes sense to set values
    width: 410
    height: 680
    // visibile must set to true - default is false
    visible: true
    //
    property bool isLandscape: width > height

    // Samsung XCover3 has 320
    property bool isSmallDevice: !isLandscape && width < 360

    property bool backKeyfreezed: false
    property bool modalPopupActive: false

    property bool myScheduleActive: false
    onMyScheduleActiveChanged: {
        if(myScheduleActive) {
            //navigationModel[2].icon = "schedule_my.png"
            drawerLoader.item.replaceIcon(scheduleNavigationIndex,"stars.png")
            drawerLoader.item.replaceText(scheduleNavigationIndex,qsTr("My Schedule"))
            favoritesLoader.item.replaceIcon(scheduleFavoritesIndex,"stars.png")
            favoritesLoader.item.replaceText(scheduleFavoritesIndex,qsTr("My Schedule"))
        } else {
            //navigationModel[2].icon = "schedule.png"
            drawerLoader.item.replaceIcon(scheduleNavigationIndex,"schedule.png")
            drawerLoader.item.replaceText(scheduleNavigationIndex,qsTr("Schedule"))
            favoritesLoader.item.replaceIcon(scheduleFavoritesIndex,"schedule.png")
            favoritesLoader.item.replaceText(scheduleFavoritesIndex,qsTr("Schedule"))
        }
    }

    // primary and accent properties:
    property variant primaryPalette: myApp.defaultPrimaryPalette()
    property color primaryLightColor: primaryPalette[0]
    property color primaryColor: primaryPalette[1]
    property color primaryDarkColor: primaryPalette[2]
    property color textOnPrimaryLight: primaryPalette[3]
    property color textOnPrimary: primaryPalette[4]
    property color textOnPrimaryDark: primaryPalette[5]
    property string iconOnPrimaryLightFolder: primaryPalette[6]
    property string iconOnPrimaryFolder: primaryPalette[7]
    property string iconOnPrimaryDarkFolder: primaryPalette[8]
    property variant accentPalette: myApp.defaultAccentPalette()
    property color accentColor: accentPalette[0]
    property color textOnAccent: accentPalette[1]
    property string iconOnAccentFolder: accentPalette[2]
    Material.primary: primaryColor
    Material.accent: accentColor
    // theme Dark vs Light properties:
    property variant themePalette: myApp.defaultThemePalette()
    property color dividerColor: themePalette[0]
    property color cardAndDialogBackground: themePalette[1]
    property real primaryTextOpacity: themePalette[2]
    property real secondaryTextOpacity: themePalette[3]
    property real dividerOpacity: themePalette[4]
    property real iconActiveOpacity: themePalette[5]
    property real iconInactiveOpacity: themePalette[6]
    property string iconFolder: themePalette[7]
    property int isDarkTheme: themePalette[8]
    property color flatButtonTextColor: themePalette[9]
    property color popupTextColor: themePalette[10]
    property real toolBarActiveOpacity: themePalette[11]
    property real toolBarInactiveOpacity: themePalette[12]
    property color toastColor: themePalette[13]
    property real toastOpacity: themePalette[14]
    // 5.7: dropShadowColor is ok - the shadow is darker as the background
    // but not so easy to distinguish as in light theme
    // optional:
    // isDarkTheme? "#E4E4E4" : Material.dropShadowColor
    property color dropShadow: Material.dropShadowColor
    onIsDarkThemeChanged: {
        if(isDarkTheme == 1) {
            Material.theme = Material.Dark
        } else {
            Material.theme = Material.Light
        }
    }
    // font sizes - defaults from Google Material Design Guide
    property int fontSizeDisplay4: 112
    property int fontSizeDisplay3: 56
    property int fontSizeDisplay2: 45
    property int fontSizeDisplay1: 34
    property int fontSizeHeadline: 24
    property int fontSizeTitle: 20
    property int fontSizeSubheading: 16
    property int fontSizeBodyAndButton: 14 // is Default
    property int fontSizeCaption: 12
    property int fontSizeActiveNavigationButton: 14
    property int fontSizeInactiveNavigationButton: 12
    // fonts are grouped into primary and secondary with different Opacity
    // to make it easier to get the right property,
    // here's the opacity per size:
    property real opacityDisplay4: secondaryTextOpacity
    property real opacityDisplay3: secondaryTextOpacity
    property real opacityDisplay2: secondaryTextOpacity
    property real opacityDisplay1: secondaryTextOpacity
    property real opacityHeadline: primaryTextOpacity
    property real opacityTitle: primaryTextOpacity
    property real opacitySubheading: primaryTextOpacity
    // body can be both: primary or secondary text
    property real opacityBodyAndButton: primaryTextOpacity
    property real opacityBodySecondary: secondaryTextOpacity
    property real opacityCaption: secondaryTextOpacity
    // using Icons as Toggle to recognize 'checked'
    property real opacityToggleInactive: 0.2
    property real opacityToggleActive: 1.0
    //
    // TODO C++ ENUM
    // Destination
    // TabBar-SwipeView
    // StackView
    property var activationPolicy: {
        "NONE":0, "IMMEDIATELY":1, "LAZY":2, "WHILE_CURRENT":3
    }

    // NAVIGATION BAR PROPRTIES (a_p == activationPolicy)
    // IMMEDIATELY: Home
    // LAZY: customer, orders
    // WHILE_CURRENT: About, Settings
    // StackView: Home --> QtPage, Settings --> primary / Accent
    // if reordering adjust properties
    property int homeNavigationIndex: 0
    property int scheduleNavigationIndex: 2
    property int speakerNavigationIndex: 3
    property int tracksNavigationIndex: 4
    property int venueNavigationIndex: 5
    property int settingsNavigationIndex: 8
    property int helpNavigationIndex: 9
    property int aboutNavigationIndex: 10
    property var navigationModel: [
        {"type": "../navigation/DrawerNavigationButton.qml", "name": "QtCon", "icon": "home.png", "source": "../pages/HomePage.qml", "showCounter":false, "showMarker":false, "a_p":1, "canGoBack":false},
        {"type": "../navigation/DrawerDivider.qml", "name": "", "icon": "", "source": "", "a_p":1, "canGoBack":false},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": qsTr("Schedule"), "icon": "schedule.png", "source": "../navigation/ScheduleNavigation.qml", "showCounter":false, "showMarker":false, "a_p":2, "canGoBack":true},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": qsTr("Speakers"), "icon": "speaker.png", "source": "../navigation/SpeakerNavigation.qml", "showCounter":false, "showMarker":false, "a_p":2, "canGoBack":true},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": "Tracks", "icon": "tag.png", "source": "../navigation/TrackNavigation.qml", "showCounter":false, "showMarker":false, "a_p":2, "canGoBack":true},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": "Venue", "icon": "venue.png", "source": "../navigation/VenueNavigation.qml", "showCounter":false, "showMarker":false, "a_p":2, "canGoBack":true},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": qsTr("Rooms"), "icon": "directions.png", "source": "../navigation/RoomsNavigation.qml", "showCounter":false, "showMarker":false, "a_p":2, "canGoBack":true},
        {"type": "../navigation/DrawerDivider.qml", "name": "", "icon": "", "source": "", "a_p":1, "canGoBack":false},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": qsTr("Settings"), "icon": "settings.png", "source": "../navigation/SettingsNavigation.qml", "showCounter":false, "showMarker":false, "a_p":3, "canGoBack":true},
        {"type": "../navigation/DrawerNavigationButton.qml", "name": qsTr("Help"), "icon": "help.png", "source": "../pages/HelpPage.qml", "showCounter":false, "showMarker":false, "a_p":3, "canGoBack":false},
        {"type": "../navigation/DrawerNavigationTextButton.qml", "name": qsTr("About this App"), "icon": "", "source": "../pages/AboutPage.qml", "showCounter":false, "showMarker":false, "a_p":3, "canGoBack":false}
    ]
    property var developerModel: {
        "type": "../navigation/DrawerNavigationButton.qml", "name": "Developer Tools", "icon": "code.png", "source": "../pages/DevToolsPage.qml", "showCounter":false, "showMarker":false, "a_p":3, "canGoBack":false
    }
    property bool initDone: false

    property var navigationTitles: [
        qsTr("QtCon 2016 Berlin"),
        "",
        qsTr("QtCon 2016 Schedule"),
        qsTr("QtCon 2016 Speakers"),
        qsTr("QtCon 2016 Tracks"),
        qsTr("QtCon 2016 Venue"),
        qsTr("QtCon 2016 Rooms"),
        "",
        qsTr("QtCon 2016 Settings"),
        qsTr("QtCon 2016 Help"),
        qsTr("QtCon 2016 About"),
        qsTr("QtCon 2016 D E V E L O P E R Tools")
    ]
    property string currentTitle: navigationTitles[navigationIndex]
    // Counter: orders
    // Marker: customer
    property var navigationData: [
        {"counter":0, "marker":""},
        {},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""},
        {"counter":0, "marker":""}
    ]
    // Menu Button
    // plus max 4 from drawer: home, customer, orders, settings
    // favoritesModel maps to index from navigationModel
    // if reordering adjust properties
    property int homeFavoritesIndex: 0
    property int scheduleFavoritesIndex: 1
    property int speakerFavoritesIndex: 2
    property int tracksFavoritesIndex: 3
    property var favoritesModel: [
        homeNavigationIndex, scheduleNavigationIndex, speakerNavigationIndex, tracksNavigationIndex
    ]
    property int firstActiveDestination: homeNavigationIndex
    property int navigationIndex: firstActiveDestination
    onNavigationIndexChanged: {
        rootPane.activateDestination(navigationIndex)
    }
    property alias navigationBar: drawerLoader.item

    property bool highlightActiveNavigationButton : true

    // NAVIGATION STYLE
    property SettingsData settings
    property int myNavigationStyle: settings? settings.navigationStyle : -1
    onMyNavigationStyleChanged: {
        if(myNavigationStyle == 2) {
            isClassicNavigationStyle = true
            isBottomNavigationStyle = false
            isComfortNavigationStyle = false
            return
        }
        if(myNavigationStyle == 1) {
            isClassicNavigationStyle = false
            isBottomNavigationStyle = true
            isComfortNavigationStyle = false
            return
        }
        isClassicNavigationStyle = false
        isBottomNavigationStyle = false
        isComfortNavigationStyle = true
    }
    property bool isClassicNavigationStyle: false
    property bool isBottomNavigationStyle: false
    property bool isComfortNavigationStyle: true
    property bool hasOnlyOneMenu: settings? (settings.oneMenuButton && isComfortNavigationStyle) : false

    // header per Page, footer global in Portrait + perhaps per Page, too
    // header and footer invisible until initDone
    footer: initDone && !isLandscape &&!isClassicNavigationStyle && drawerLoader.status == Loader.Ready && navigationBar.position == 0 ? favoritesLoader.item : null
    header: (isLandscape && !useDefaultTitleBarInLandscape) || !initDone ? null : titleBar
    // show TITLE  BARS is delayed until INIT DONE
    property bool useDefaultTitleBarInLandscape: false
    Loader {
        id: titleBar
        visible: (!isLandscape || useDefaultTitleBarInLandscape) && initDone
        active: (!isLandscape || useDefaultTitleBarInLandscape) && initDone
        source: "navigation/DrawerTitleBar.qml"
    }
    // in LANDSCAPE header is null and we have a floating TitleBar
    Loader {
        id: titleBarFloating
        visible: !useDefaultTitleBarInLandscape && isLandscape && initDone
        active: !useDefaultTitleBarInLandscape && isLandscape && initDone
        source: "navigation/DrawerTitleBar.qml"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    // end TITLE BARS
    function resetDefaultTitleBarInLandscape() {
        useDefaultTitleBarInLandscape = false
    }
    function setDefaultTitleBarInLandscape() {
        useDefaultTitleBarInLandscape = true
    }

    // STACK VIEW (rootPane)
    // the ROOT contains always only one Page,
    // which will be replaced if root node changed
    StackView {
        id: rootPane
        focus: true
        anchors.top: isLandscape ? titleBarFloating.bottom : parent.top
        anchors.left: parent.left
        anchors.topMargin: isLandscape ? 6 : 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // STACK VIEW TRANSITIONS
        replaceEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 300
            }
        }
        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 300
            }
        }
        // end STACKVIEW TRANSITIONS

        // STACK VIEW KEYS and SHORTCUTS
        // support of BACK key
        // can be used from StackView pushed on ROOT
        // or to exit the app
        // https://wiki.qt.io/Qt_for_Android_known_issues
        // By default the Back key will terminate Qt for Android apps, unless the key event is accepted.
        Keys.onBackPressed: {
            event.accepted = true

            if(appWindow.backKeyfreezed) {
                showToast(qsTr("Please wait. Back key not allowed at the moment."))
                return
            }
            if(appWindow.modalPopupActive) {
                showToast(qsTr("Back key not allowed - please select an option."))
                return
            }
            if(!initDone) {
                return
            }

            if(navigationModel[navigationIndex].canGoBack && destinations.itemAt(navigationIndex).item.depth > 1) {
                destinations.itemAt(navigationIndex).item.goBack()
                return
            }
            if (Qt.platform.os === "winrt") {
                Qt.quit()
                return
            }
            if (Qt.platform.os === "android") {
                popupExitApp.open()
                return
            }
            showToast(qsTr("No more Pages"))
        }
        // TODO some Shortcuts
        // end STACK VIEW KEYS and SHORTCUTS

        // STACK VIEW INITIAL ITEM (BUSY INDICATOR)
        // immediately activated and pushed on stack as initialItem
        Loader {
            id: initialPlaceholder
            property bool isUpdate: false
            source: "pages/InitialItemPage.qml"
            active: true
            visible: false
            onLoaded: {
                // Show BUSY INDICATOR
                if(isUpdate) {
                    rootPane.replace(item)
                    item.update()
                    // now doing the UPDATE stuff
                    updateTimer.start()
                } else {
                    rootPane.initialItem = item
                    item.init()
                    // Now something is VISIBLE - do the other time-consuming stuff
                    startupDelayedTimer.start()
                }
            }
        }
        // end STACK VIEW INITIAL ITEM

        // U P D A T E
        Timer {
            id: updateTimer
            interval: 300
            repeat: false
            onTriggered: {
                // cleanup all running stuff in QML
                destinations.model = []
                // start update schedule (C++)
                dataUtil.startUpdate()
            }
        } // updateTimer
        function startUpdate() {
            myScheduleActive = false
            // remove drawer and bottom navigation
            initDone = false
            // replace root item
            initialPlaceholder.isUpdate = true
            // updateTimer started from initialPlaceholder
            initialPlaceholder.active = true
        }
        PopupUpdate {
            id: updatePopup
            modal: true
            closePolicy: Popup.NoAutoClose
            onClosed: {
                // read all data
                startupDelayedTimer.start()
            }
        } // updatePopup
        function updateFailed(message) {
            dataUtil.reloadData()
            // info and reload prev stuff
            updatePopup.text = message
            updatePopup.buttonsVisible = true
            updatePopup.isUpdate = false
            updatePopup.open()
        }
        Timer {
            id: updateDoneTimer
            interval: 2500
            repeat: false
            onTriggered: {
                // read all data
                startupDelayedTimer.start()
            }
        } // updateTimer
        function updateDone() {
            // read all data
            updateDoneTimer.start()
        }
        function showUpdateProgress(progressInfo) {
            initialPlaceholder.item.showProgress(progressInfo)
        }
        Connections {
            target: dataUtil
            onProgressInfo: rootPane.showUpdateProgress(progressInfo)
        }
        Connections {
            target: dataUtil
            onUpdateDone: rootPane.updateDone()
        }
        Connections {
            target: dataUtil
            onUpdateFailed: rootPane.updateFailed(message)
        }
        // END   U P D A T E

        // DELAYED STARTUP TIMER
        // do the hevy stuff while initialItem is visible
        // initialize Data, create Navigation, make Title visible, ...
        Timer {
            id: startupDelayedTimer
            interval: 300
            repeat: false
            onTriggered: {
                console.log("startupDelayedTimer START")
                initialPlaceholder.item.showInfo("Initialize Data ...")
                if(!initialPlaceholder.isUpdate) {
                    dataManager.init()
                    settings = dataManager.settingsData()
                }
                dataUtil.setSessionFavorites()
                dataManager.resolveReferencesForAllSpeaker()
                dataManager.resolveReferencesForAllSession()
                dataUtil.resolveSessionsForSchedule()
                initialPlaceholder.item.showInfo("Create Navigation Controls ...")
                // add navigation model for DEBUG BUILD ?
                if(myApp.isDebugBuild() && !initialPlaceholder.isUpdate) {
                    console.log("DEBUG BUILD added as destination")
                    // special mode for ekke testing the app
                    // navigationModel.push(developerModel)
                }
                // inject model into Destinations Repeater
                destinations.model = navigationModel
                // show the Navigation Bars (Drawer and Favorites)
                initDone = true
                // now NavigationBars available, we can update counters:
                // rootPane.updateOrderCounter()
                // show first destination (should always be IMMEDIATELY)
                rootPane.activateDestination(firstActiveDestination)
                console.log("startupDelayedTimer DONE")
            }
        }

        // ASYNC STARTUP: Destinations will be loaded from Timer
        // that's why no model is attached at the beginning
        // startupDelayedTimer will set the model
        Repeater {
            id: destinations
            // model: navigationModel
            // Destination encapsulates Loader
            // depends from activationPolicy how to load dynamically
            Destination {
                id: destinationLoader
            }
            // Repeater creates all destinations (Loader)
            // all destinatation items w activationPolicy IMMEDIATELY are activated
        }

        // STACK VIEW (rootPane) FUNCTIONS
        // switch to new Destination
        // Destinations are lazy loaded via Loader
        function activateDestination(navigationIndex) {
            if(destinations.itemAt(navigationIndex).status == Loader.Ready) {
                console.log("replace item on root stack: "+navigationIndex)
                replaceDestination(destinations.itemAt(navigationIndex))
            } else {
                console.log("first time item to be replaced: "+navigationIndex)
                destinations.itemAt(navigationIndex).active = true
            }
        }
        // called from activeDestination() and also from Destination.onLoaded()
        function replaceDestination(theItemLoader) {
            var previousIndex = rootPane.currentItem.myIndex
            var previousItemLoader
            if(previousIndex >= 0) {
                previousItemLoader  = destinations.itemAt(previousIndex)
            }
            // because of https://bugreports.qt.io/browse/QTBUG-54260
            // remember currentIndex before being replaced
            console.log("replace destination for name: "+rootPane.currentItem.name)
            if(rootPane.currentItem.name == "ScheduleNavPage") {
                if(!appWindow.myScheduleActive) {
                    rootPane.currentItem.lastCurrentIndex = rootPane.currentItem.getCurrentIndex()
                    console.log("dayNavPage remember "+rootPane.currentItem.getCurrentIndex())
                }
            }
            // reset currentIndex to the last one
            if(theItemLoader.item.name == "ScheduleNavPage") {
                //theItemLoader.item.currentIndex = theItemLoader.item.lastCurrentIndex
                // the SwipeView is one level deeper, so we delegate this to the next StackView
                if(!appWindow.myScheduleActive) {
                    theItemLoader.item.setCurrentIndex()
                }
            }

            // here you can call work to be done if user changes destination
            // should also be called if app will be paused or exit
            if(rootPane.currentItem.name == "ScheduleNavPage") {
                // TODO do this for all stackViews on top of root StackView
                rootPane.currentItem.destinationAboutToChange()
            }

            // now replace the Page
            rootPane.replace(theItemLoader.item)
            // check if previous should be unloaded

            if(previousIndex >= 0) {
                if(destinations.itemAt(previousIndex).pageActivationPolicy == activationPolicy.WHILE_CURRENT) {
                    destinations.itemAt(previousIndex).active = false
                }
            } else {
                initialPlaceholder.active = false
            }
        }

        // example HowTo set a counter
        // first time called from startupDelayedTimer
        function updateOrderCounter() {
            //            var counter = dataManager.orderPropertyList.length
            //            navigationData[4].counter = counter
            //            navigationBar.navigationButtons.itemAt(4).item.counter = counter
        }
        // update counter if Orders deleted or added
        // connect C++ SIGNAL to QML SLOT
        //        Connections {
        //                target: dataManager
        //                onOrderPropertyListChanged: rootPane.updateOrderCounter()
        //            }

        // example HowTo set a marker
        function updateCustomerMarker(abc) {
            //            switch(abc) {
            //                case 0:
            //                    navigationData[3].marker = "green"
            //                    break;
            //                case 1:
            //                    navigationData[3].marker = "grey"
            //                    break;
            //                case 2:
            //                    navigationData[3].marker = "red"
            //                    break;
            //                default:
            //                    navigationData[3].marker = "transparent"
            //            }
            //            navigationBar.navigationButtons.itemAt(3).item.marker = navigationData[3].marker
        }
        // end STACKVIEW FUNCTIONS

    } // rootPane

    // INIT DONE: show TITLE and NAVIGATION BARS

    // NAVIGATION BARS (DRAWER and FAVORITES)
    // The sliding Drawer
    // there's an alias in appWindow: navigationBar --> drawerLoader.item
    Loader {
        id: drawerLoader
        active: initDone
        visible: initDone
        source: "navigation/DrawerNavigationBar.qml"
    }

    Loader {
        id: favoritesLoader
        active: initDone
        // attention: set also footer !
        visible: initDone && !isLandscape && !isClassicNavigationStyle && drawerLoader.status == Loader.Ready && navigationBar.position == 0
        source: "navigation/DrawerFavoritesNavigationBar.qml"
    }
    function openNavigationBar() {
        navigationBar.open()
    }
    function closeNavigationBar() {
        navigationBar.close()
    }
    // end NAVIGATION BARS

    // APP WINDOW FUNCTIONS

    function switchPrimaryPalette(paletteIndex) {
        primaryPalette = myApp.primaryPalette(paletteIndex)
    }
    function switchAccentPalette(paletteIndex) {
        accentPalette = myApp.accentPalette(paletteIndex)
    }

    // we can loose the focus if Menu or Popup is opened
    function resetFocus() {
        rootPane.focus = true
    }

    function showToast(info) {
        popupToast.start(info)
    }
    function showError(info) {
        popupError.start(info)
    }

    function showInfo(info) {
        popupInfo.text = info
        popupInfo.buttonText = qsTr("OK")
        popupInfo.open()
    }
    // end APP WINDOW FUNCTIONS

    // APP WINDOW POPUPS
    PopupExit {
        id: popupExitApp
        onAboutToHide: {
            popupExitApp.stopTimer()
            resetFocus()
            if(popupExitApp.isExit) {
                Qt.quit()
            }
        }
    } // popupExitApp

    PopupInfo {
        id: popupInfo
        onAboutToHide: {
            popupInfo.stopTimer()
            resetFocus()
        }
    } // popupInfo
    // PopupToast
    PopupToast {
        id: popupToast
        onAboutToHide: {
            resetFocus()
        }
    } // popupToast
    // PopupToast
    PopupError {
        id: popupError
        onAboutToHide: {
            resetFocus()
        }
    } // popupError
    // end APP WINDOW POPUPS

} // app window
