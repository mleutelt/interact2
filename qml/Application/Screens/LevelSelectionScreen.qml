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
            // TODO: factor this out into a component that can be reused
            delegate: Button {
                implicitWidth: GridView.view.cellWidth
                implicitHeight: GridView.view.cellHeight
                contentItem: ColumnLayout {
                    Image {
                        id: previewImage

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        fillMode: Image.PreserveAspectCrop
                        source: model.preview

                        BusyIndicator {
                            anchors.centerIn: parent
                            running: App.levelHandler.loading && model.name === App.levelHandler.currentLevelData.name
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        horizontalAlignment: Label.AlignHCenter
                        verticalAlignment: Label.AlignVCenter
                        text: model.name
                    }
                }

                onClicked: App.levelHandler.loadLevel(model.path)
            }
        }
        GridView {
            model: App.levelHandler.userLevels
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
