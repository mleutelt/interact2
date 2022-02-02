import QtQuick

import App

import Box2D

Rectangle {
    id: container

    property color itemColor: "black"
    property bool interactive: true
    property bool hovered: false
    property bool clickEnabled: false
    property var clickHandler
    property bool hoverEnabled: false
    property var hoverHandler
    property bool dragEnabled: false
    property var dragHandler
    property bool wheelEnabled: false
    property var wheelHandler
    property bool pinchEnabled: false
    property var pinchHandler
    property alias visualItem: childContainer.children
    property bool soundsEnabled: true
    property bool showBoundingBox: false
    property var contactHandler
    property bool isGameItem: false
    property bool isGoalItem: false
    property bool invisible: false

    property alias physicalObject: body.fixtures
    property Body body: Body {
        id: body

        target: container
    }

    // Body properties
    property alias world: body.world
    property alias linearDamping: body.linearDamping
    property alias angularDamping: body.angularDamping
    property alias bodyType: body.bodyType
    property alias bullet: body.bullet
    property alias sleepingAllowed: body.sleepingAllowed
    property alias fixedRotation: body.fixedRotation
    property alias active: body.active
    property alias awake: body.awake
    property alias linearVelocity: body.linearVelocity
    property alias angularVelocity: body.angularVelocity
    property alias fixtures: body.fixtures
    property alias gravityScale: body.gravityScale

    signal beginContact(other: Fixture)
    signal endContact(other: Fixture)

    color: App.debugMode || showBoundingBox ? "red" : "transparent"
    opacity: App.debugMode || showBoundingBox ? 0.5 : 1

    implicitWidth: childContainer.childrenRect.width
    implicitHeight: childContainer.childrenRect.height

    Item {
        id: childContainer

        anchors.fill: parent
    }

    Column {
        anchors.centerIn: parent
        visible: App.debugMode
        rotation: -container.rotation

        Text {
            font.pixelSize: 8
            text: "game item:" + container.isGameItem
        }
        Text {
            font.pixelSize: 8
            text: "goal item:" + container.isGoalItem
        }
        Text {
            font.pixelSize: 8
            text: "sounds:" + container.soundsEnabled
        }
        Text {
            font.pixelSize: 8
            text: "interactive:" + container.interactive
        }
    }

    onBeginContact: other => {
        if (container.contactHandler && typeof container.contactHandler === "function")
            container.contactHandler(container, other.getBody().target)
    }

    Component.onCompleted: {
        if (soundsEnabled)
            Sound.playSound(Sound.Plopp)
    }
}
