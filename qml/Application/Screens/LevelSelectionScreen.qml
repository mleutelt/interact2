import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Page {
    id: container

    header: TabBar {
        id: tabBar

        TabButton {
            text: "Game levels"
        }
        TabButton {
            text: "User levels"
        }
    }
    footer: Button {
        text: "Back"

        onClicked: Screens.showPrevious()
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        GridView {
            enabled: !App.levelHandler.loading
            clip: true
            model: App.levelHandler.availableLevels
            cellWidth: container.width / 3
            cellHeight: cellWidth
            delegate: ImageButton {
                implicitWidth: GridView.view.cellWidth
                implicitHeight: GridView.view.cellHeight
                loading: App.levelHandler.loading && model.name === App.levelHandler.currentLevelData.name
                image: model.preview
                text: model.name

                onClicked: App.levelHandler.loadLevel(model.path)
            }
        }

        GridView {
            enabled: !App.levelHandler.loading
            clip: true
            model: App.levelHandler.userLevels
            cellWidth: container.width / 3
            cellHeight: cellWidth
            delegate: ImageButton {
                implicitWidth: GridView.view.cellWidth
                implicitHeight: GridView.view.cellHeight
                loading: App.levelHandler.loading && model.name === App.levelHandler.currentLevelData.name
                image: model.preview
                text: model.name

                onClicked: App.levelHandler.loadLevel(model.path)
            }
        }
    }

    Connections {
        target: App.levelHandler

        // NOTE: change screen as soon as level has been loaded
        // Maybe use a different signal here?
        function onLoadingChanged() {
            if (!App.levelHandler.loading) {
                Screens.show(Screens.CurrentLevel)
            }
        }
    }
}
