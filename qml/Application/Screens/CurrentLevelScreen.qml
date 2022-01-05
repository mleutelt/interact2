import QtQuick
import QtQuick.Controls

import App

Page {
    id: container

    Level {
        id: level

        backgroundImage.source: App.levelHandler.currentLevelData.backgroundImage
        objectFactory.model: App.levelHandler.currentLevelData
    }

    RoundButton {
        id: exitButton

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 10
        text: "Exit"

        onClicked: Screens.showInitial()
    }
}
