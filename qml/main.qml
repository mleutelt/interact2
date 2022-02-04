import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

ApplicationWindow {
    id: applicationWindow

    title: Application.name
    minimumHeight: Style.application.minimumWindowSize.height
    minimumWidth: Style.application.minimumWindowSize.width
    height: Style.application.preferredWindowSize.height
    width: Style.application.preferredWindowSize.width
    flags: Style.application.windowFlags
    visibility: Style.application.windowVisibility
    visible: true

    Shortcut {
        sequence: "m"

        onActivated: {
            toggleMusicAction.toggle()
            Notification.showText(qsTr("Music is %1").arg(Sound.musicPlaying ? "on" : "off"))
        }
    }

    Action {
        id: toggleMusicAction

        onToggled: {
            if (Sound.musicPlaying) {
                Sound.stopMusic()
            } else {
                Sound.playMusic()
            }
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent
//        layer.enabled: true
//        layer.effect: ShaderEffect {
//            fragmentShader: "qrc:/resources/shaders/fragment.frag.qsb"
//        }

        onDepthChanged: Sound.playSound(Sound.PaperSlide)
    }

    // TODO: implement nice shader effect
//    ShaderEffect {
//        property variant src: effectSource

//        anchors.fill: parent
//        vertexShader: "qrc:/resources/shaders/vertex.vert.qsb"
//        fragmentShader: "qrc:/resources/shaders/fragment.frag.qsb"
//    }

//    ShaderEffectSource {
//        id: effectSource
//        sourceItem: stackView
//        hideSource: true
//        live: true
//    }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: "\uf085"
        visible: Screens.currentScreen !== Screens.LevelSelection

        onClicked: settingsDialog.open()
    }

    RoundButton {
        id: exitButton

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
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

    Connections {
        target: Notification

        function onShowText(text) {
            notification.text = text
            notificationAnimation.restart()
        }
    }

    Connections {
        target: App.levelHandler

        function onNextLevelAvailable(level) {
            Screens.show(Screens.CurrentLevel)
            App.levelHandler.loadLevel(level)
            Notification.showText(App.levelHandler.currentLevelData.name)
        }

        function onNextLevelNotAvailable() {
            Screens.show(Screens.Main)
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
                        text: qsTr("Show debug infos")
                        checked: App.debugMode

                        onToggled: App.debugMode = !App.debugMode
                    }

                    CheckBox {
                        text: qsTr("Show Box2d debug info")
                        checked: App.debugBox2d

                        onToggled: App.debugBox2d = !App.debugBox2d
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

                        onToggled: toggleMusicAction.toggle()
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

                    CheckBox {
                        id: soundEffectsCheckBox
                        text: qsTr("Sound effects")
                        checked: Sound.effectsEnabled

                        onToggled: Sound.effectsEnabled = !Sound.effectsEnabled
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: qsTr("Volume")
                        }

                        Slider {
                            Layout.fillWidth: true

                            enabled: soundEffectsCheckBox.checked
                            value: Sound.effectsVolume

                            onValueChanged: Sound.effectsVolume = value
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

    Label {
        id: notification

        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 40
        font.bold: true

        OpacityAnimator on opacity {
            id: notificationAnimation
            from: 1
            to: 0
            duration: 3000
        }
    }

    Component.onCompleted: Screens.show(Screens.Main)
}
