import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: roundRectangle

    property alias density: circle.density
    property alias friction: circle.friction
    property alias restitution: circle.restitution
    property alias sensor: circle.sensor
    property alias categories: circle.categories
    property alias collidesWith: circle.collidesWith
    property alias groupIndex: circle.groupIndex

    physicalObject: B2D.Circle {
        id: circle

        radius: roundRectangle.radius
        density: 1
        friction: 0.5
        restitution: 0.3

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
    }
    visualItem: Rectangle {
        id: roundRectangle

        anchors.fill: parent
        antialiasing: true
        radius: container.width / 2
        color: container.itemColor
        border.color: hoverHandler.hovered ? "red" : "transparent"
        border.width: hoverHandler.hovered ? 2 : 0
        containmentMask: QtObject {
            function contains(point: point) : bool {
                return (Math.pow(point.x - roundRectangle.radius, 2) + Math.pow(point.y - roundRectangle.radius, 2)) < Math.pow(roundRectangle.radius, 2)
            }
        }

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
