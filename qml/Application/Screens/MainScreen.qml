import QtQuick
import QtQuick.Controls

import App
import Box2D

Page {
    id: container

    Component {
        id: ball

        PCircle {
            bullet: true
            bodyType: Body.Dynamic
            density: 0.9
            friction: 0.9
            restitution: 0.2

            item.color: Qt.rgba(Math.random(), Math.random(), Math.random())
        }
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
            text: "Interact 2"
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
            var newBall = ball.createObject(container)
            newBall.width = 20 + 20 * Math.random()
            newBall.height = newBall.width
            newBall.x = ApplicationWindow.window.width / 2 + newBall.width * Math.random()
            newBall.y = -50
            var impulse = 5
            var impulseX = 0
            var impulseY = impulse
            newBall.body.applyLinearImpulse(Qt.point(impulseX, -impulseY), newBall.body.getWorldCenter())
        }
    }
}
