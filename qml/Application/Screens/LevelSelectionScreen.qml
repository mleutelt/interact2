import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Page {
    id: container

    property var levelSelectionHandler

    footer: TabBar {
        id: tabBar

        position: TabBar.Footer

        TabButton {
            font.capitalization: Font.Capitalize
            text: qsTr("Game levels")
        }
        TabButton {
            font.capitalization: Font.Capitalize
            text: qsTr("User levels")
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        GridView {
            clip: true
            model: App.levelHandler.availableLevels
            cellWidth: container.width / 3
            cellHeight: cellWidth
            delegate: Item {
                implicitWidth: GridView.view.cellWidth
                implicitHeight: GridView.view.cellHeight

                ImageButton {
                    anchors.fill: parent
                    anchors.margins: 10
                    image: model.preview
                    text: model.name

                    onClicked: container.levelSelectionHandler(model.path)
                }
            }
        }

        GridView {
            clip: true
            model: App.levelHandler.userLevels
            cellWidth: container.width / 3
            cellHeight: cellWidth
            delegate: Item {
                implicitWidth: GridView.view.cellWidth
                implicitHeight: GridView.view.cellHeight

                ImageButton {
                    anchors.fill: parent
                    anchors.margins: 10
                    image: model.preview
                    text: model.name

                    onClicked: container.levelSelectionHandler(model.path)
                }
            }
        }
    }
}
