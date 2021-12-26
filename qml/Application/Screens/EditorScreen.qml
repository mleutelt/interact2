import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Page {
    id: container

    footer: Button {
        text: "Back"

        onClicked: Screens.showPrevious()
    }

    // TODO: figure out editor controls
    // - element selection (which physical element should be placed when clicking)
    // - element modification (delete etc.)
    // - save level to file
    // - load existing level from file
    ColumnLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10

        RoundButton {
        }
        RoundButton {
        }
        RoundButton {
        }
        RoundButton {
        }
    }
}
