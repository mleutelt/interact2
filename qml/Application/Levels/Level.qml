import QtQuick

import App

import Box2D

Item {
    id: container

    property World physicsWorld: World {}
    property alias ground: ground
    property alias leftWall: leftWall
    property alias rightWall: rightWall
    property alias topWall: topWall

    anchors.fill: parent

    PRectangle {
        id: ground

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 40

        friction: 1
        density: 1
        world: enabled ? physicsWorld : null
    }

    PRectangle {
        id: topWall

        height: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        world: enabled ? physicsWorld : null
    }

    PRectangle {
        id: leftWall

        width: 0
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40

        world: enabled ? physicsWorld : null
    }

    PRectangle {
        id: rightWall

        width: 0
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40

        world: enabled ? physicsWorld : null
    }
}
