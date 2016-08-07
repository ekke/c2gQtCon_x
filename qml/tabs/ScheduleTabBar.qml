// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import "../common"

TabBar {
    id: myTabBar
    Layout.fillWidth: true
    currentIndex: 0
    onCurrentIndexChanged: {
        console.log("Tab Bar current index changed: "+ currentIndex)
        navSwipePane.currentIndex = currentIndex // navPane
    }
    Repeater {
        model: dataManager.dayPropertyList.length
        TabButton {
            focusPolicy: Qt.NoFocus
            text: dataUtil.scheduleTabName(index)
            width: tabBarIsFixed? myTabBar.width / model.length  : Math.max(112, myTabBar.width / model.length)
        }
    } // repeater

}
