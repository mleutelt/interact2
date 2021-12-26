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
            clip: true
            model: App.levelHandler.availableLevels
            cellWidth: container.width / 3
            cellHeight: cellWidth
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
                    }
                    Label {
                        Layout.fillWidth: true
                        horizontalAlignment: Label.AlignHCenter
                        verticalAlignment: Label.AlignVCenter
                        text: model.name
                    }
                }

                onClicked: {
                    Screens.show(Screens.CurrentLevel)
                    App.levelHandler.loadLevel(model.path)
                }
            }
        }
        GridView {
            model: App.levelHandler.userLevels
        }
    }
}
