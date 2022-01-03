import QtQuick
import QtQuick.Controls

import App
import Box2D

Page {
    id: container

    function spawnBall(position, size) {
        var newBall = ball.createObject(level)
        newBall.width = size.width
        newBall.height = newBall.width
        newBall.x = position.x
        newBall.y = position.y
    }

    Component {
        id: ball

        PCircle {
            bullet: true
            bodyType: Body.Dynamic
            restitution: 0.5

            item.color: Qt.rgba(Math.random(), Math.random(), Math.random())
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: spawnBall(Qt.point(mouseX - 15, mouseY - 15), Qt.size(30, 30))
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
            text: "interact 2"

        }

        PStar {
            bodyType: Body.Static
            x: title.x - width / 3
            y: title.y
            width: 50
            height: 50
        }

        PRectangle {
            id: startButton

            anchors.centerIn: parent

            visualItem: Button {
                implicitWidth: container.width / 5
                text: "Start"

                onClicked: Screens.show(Screens.LevelSelection)
            }
        }

        PRectangle {
            id: editorButton

            anchors.top: startButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: startButton.horizontalCenter

            visualItem: Button {
                implicitWidth: container.width / 4
                text: "Editor"

                onClicked: Screens.show(Screens.Editor)
            }
        }

        PRectangle {
            id: settingsButton

            anchors.top: editorButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: editorButton.horizontalCenter

            visualItem: Button {
                implicitWidth: container.width / 3
                text: "Settings"

                onClicked: Screens.show(Screens.Settings)
            }
        }
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: Application.version
    }

    StackView.onActivated: {
        for (let i = 0; i < 5; i++) {
            var width = 20 + 20 * Math.random()
            var size = Qt.size(width, width)
            var position = Qt.point(ApplicationWindow.window.width / 2 + width * Math.random(), -size.height)

            spawnBall(position, size)
        }
    }
}
