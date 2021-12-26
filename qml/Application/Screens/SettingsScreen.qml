import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Page {
    id: container

    padding: 10
    header: Label {
        padding: 10
        text: "Settings"
    }

    footer: Button {
        text: "Back"

        onClicked: Screens.showPrevious()
    }

    GridLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        columns: 2

        GroupBox {
            Layout.fillWidth: true
            title: "Application"

            ColumnLayout {
                anchors.fill: parent

                // TODO: language selection etc.
                ComboBox {
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: "Debugging"

            ColumnLayout {
                anchors.fill: parent

                CheckBox {
                    text: "Debug mode"
                    checked: App.debugMode

                    onToggled: App.debugMode = !App.debugMode
                }
            }
        }
    }
}
