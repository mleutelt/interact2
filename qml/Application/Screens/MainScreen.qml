import QtQuick
import QtQuick.Controls

import App

import Box2D

Page {
    id: container

    background.opacity: 0

    Component {
        id: ball

        PCircle {
            bullet: true
            bodyType: Body.Dynamic
            restitution: 0.5

            itemColor: Qt.rgba(Math.random(), Math.random(), Math.random())
            interactive: false
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: ball.createObject(level, {
                                         x: mouseX - 15,
                                         y: mouseY - 15,
                                         width: 30,
                                         height: 30
                                     })
    }

    Level {
        id: level

        topWall.enabled: false

        Text {
            id: title

            anchors.bottom: startButton.top
            anchors.bottomMargin: 60
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 82
            text: qsTr("interact 2")

        }

        PStar {
            bodyType: Body.Static
            x: title.x - width / 3
            y: title.y
            width: 50
            height: 50
            soundsEnabled: false

            onBeginContact: Sound.playSound(Sound.Bell)
        }

        PRectangle {
            id: startButton

            anchors.centerIn: parent
            soundsEnabled: false
            itemColor: Qt.rgba(Math.random(), Math.random(), Math.random())

            visualItem: Button {
                implicitWidth: container.width / 5
                text: qsTr("Start")

                onClicked: Screens.show(Screens.LevelSelection, {
                                            levelSelectionHandler: (path) => {
                                                Screens.show(Screens.CurrentLevel)
                                                App.levelHandler.loadLevel(path)
                                                Notification.showText(App.levelHandler.currentLevelData.name)
                                            }
                                        })
            }
        }

        PRectangle {
            id: editorButton

            anchors.top: startButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: startButton.horizontalCenter
            soundsEnabled: false
            itemColor: Qt.rgba(Math.random(), Math.random(), Math.random())

            visualItem: Button {
                implicitWidth: container.width / 4
                text: qsTr("Editor")

                onClicked: Screens.show(Screens.Editor)
            }
        }
    }

    StackView.onActivated: {
        Sound.playMusic(App.musicPath + "Lullaby.mp3")

        for (let i = 0; i < 5; i++) {
            var width = 20 + 20 * Math.random()
            var size = Qt.size(width, width)
            var position = Qt.point(ApplicationWindow.window.width / 2 + width * Math.random(), -size.height)

            ball.createObject(level, {
                                  width: size.width,
                                  height: size.height,
                                  x: position.x,
                                  y: position.y,
                                  soundsEnabled: false,
                              })
        }
    }
}
