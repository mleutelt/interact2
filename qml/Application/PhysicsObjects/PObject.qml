import QtQuick

import App

import Box2D

Rectangle {
    id: container

    property color itemColor: "black"
    property bool interactive: true
    property var interactionHandler
    property alias visualItem: childContainer.children
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

    color: App.debugMode ? "red" : "transparent"
    opacity: App.debugMode ? 0.5 : 1

    implicitWidth: childContainer.childrenRect.width
    implicitHeight: childContainer.childrenRect.height

    TapHandler {
        onTapped: {
            if (container.interactive && interactionHandler && typeof interactionHandler === "function")
                container.interactionHandler(index)
        }
    }

    Item {
        id: childContainer

        anchors.fill: parent
    }
}
