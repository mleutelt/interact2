import QtQuick

import Box2D as B2D

PObject {
    id: container

    property alias item: rectangle

    property alias density: box.density
    property alias friction: box.friction
    property alias restitution: box.restitution
    property alias sensor: box.sensor
    property alias categories: box.categories
    property alias collidesWith: box.collidesWith
    property alias groupIndex: box.groupIndex

    physicalObject: B2D.Box {
        id: box

        width: container.width
        height: container.height
        density: 1
        friction: 0.5
        restitution: 0.1

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
    }
    visualItem: Rectangle {
        id: rectangle

        anchors.fill: parent
        antialiasing: true
        color: container.itemColor
        border.color: hoverHandler.hovered ? "red" : "transparent"
        border.width: hoverHandler.hovered ? 2 : 0

        HoverHandler {
            id: hoverHandler

            enabled: container.interactive
        }

        TapHandler {
            enabled: container.interactive

            onTapped: {
                if (container.interactionHandler && typeof container.interactionHandler === "function")
                    container.interactionHandler(index)
            }
        }
    }
}
