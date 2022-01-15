import QtQuick

import App

import Box2D

Item {
    id: container

    property alias physicsWorld: objectFactory.world
    property alias ground: ground
    property alias leftWall: leftWall
    property alias rightWall: rightWall
    property alias topWall: topWall
    property alias objectFactory: objectFactory
    property alias backgroundImage: backgroundImage

    anchors.fill: parent

    Image {
        id: backgroundImage

        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    PRectangle {
        id: ground

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 0

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

        world: enabled ? physicsWorld : null
    }

    PRectangle {
        id: rightWall

        width: 0
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        world: enabled ? physicsWorld : null
    }

    PObjectFactory {
        id: objectFactory

        world: World {}
        level: container
    }

    DebugDraw {
        id: debugDraw

        anchors.fill: parent
        world: physicsWorld
        opacity: 0.7
        visible: App.debugBox2d
    }
}
