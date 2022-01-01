import QtQuick
import QtQuick.Controls

import App

Page {
    id: container

//    footer: Button {
//        text: "Exit level"

//        onClicked: Screens.showInitial()
//    }

    Level {
        id: level

        objectFactory.model: App.levelHandler.currentLevelData
    }
}
