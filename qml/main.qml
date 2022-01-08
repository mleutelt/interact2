import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

ApplicationWindow {
    id: applicationWindow

    title: Application.name
    minimumHeight: Style.application.minimumWindowSize.height
    minimumWidth: Style.application.minimumWindowSize.width
    visible: true

    StackView {
        id: stackView

        anchors.fill: parent
    }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        font.family: Style.fontAwesomeRegular.font.family
        font.weight: Font.Regular
        text: "\uf085"
        visible: Screens.currentScreen !== Screens.LevelSelection

        onClicked: settingsDialog.open()
    }

    RoundButton {
        id: exitButton

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        font.family: Style.fontAwesomeRegular.font.family
        font.weight: Font.Regular
        text: "\uf060"
        visible: Screens.currentScreen !== Screens.Main

        onClicked: Screens.showInitialScreen()
    }

    Connections {
        target: Screens

        function onShowScreen(screen, properties) {
            stackView.push(Screens.file(screen), properties)
        }

        function onShowPreviousScreen() {
            stackView.pop()
        }

        function onShowInitialScreen() {
            Screens.currentScreen = Screens.Main
            stackView.pop(null)
        }
    }

    Dialog {
        id: settingsDialog

        anchors.centerIn: parent
        modal: true

        GridLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            columns: 2

            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("Application")

                ColumnLayout {
                    anchors.fill: parent

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            Layout.fillWidth: true
                            text: qsTr("Language")
                        }

                        // TODO: language selection etc.
                        ComboBox {
                        }
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("Debugging")

                ColumnLayout {
                    anchors.fill: parent

                    CheckBox {
                        text: qsTr("Show physics objects")
                        checked: App.debugMode

                        onToggled: App.debugMode = !App.debugMode
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "v%1".arg(Application.version)
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("Music")

                ColumnLayout {
                    anchors.fill: parent

                    CheckBox {
                        id: backgroundMusicCheckBox
                        text: qsTr("Background music")
                        checked: Sound.musicPlaying

                        onToggled: {
                            if (Sound.musicPlaying) {
                                Sound.stopMusic()
                            } else {
                                Sound.playMusic()
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: qsTr("Volume")
                        }

                        Slider {
                            Layout.fillWidth: true

                            enabled: backgroundMusicCheckBox.checked
                            value: Sound.musicVolume

                            onValueChanged: Sound.musicVolume = value
                        }
                    }
                }
            }
        }
    }

    Item {
        id: pointerArea

        anchors.fill: parent

        PointHandler {
            id: handler
            target: Rectangle {
                parent: pointerArea
                visible: handler.active
                color: "transparent"
                width: 30
                height: width
                radius: width / 2
                border.width: 1
                border.color: "black"
                x: handler.point.position.x - width / 2
                y: handler.point.position.y - height / 2
            }
        }
    }

    Component.onCompleted: {
        Screens.show(Screens.Main)
        Sound.playMusic("Lullaby.mp3")
    }
}
