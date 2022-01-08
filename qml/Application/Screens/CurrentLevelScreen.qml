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
}
