import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Page {
    id: container

    property var levelSelectionHandler

    clip: true
    background.opacity: 0
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
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        currentIndex: tabBar.currentIndex

        GridView {
            model: App.levelHandler.availableLevels
            cellWidth: parent.width / 3
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
            model: App.levelHandler.userLevels
            cellWidth: parent.width / 3
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

                    ToolButton {
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        font: Style.fontAwesomeSolid.font
                        flat: true
                        text: "\uf2ed"

                        onClicked: App.levelHandler.deleteLevel(index)
                    }
                }
            }
        }
    }
}
