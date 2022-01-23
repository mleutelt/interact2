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
        border.color: container.hovered ? "red" : "transparent"
        border.width: container.hovered ? 2 : 0
        containmentMask: QtObject {
            function contains(point: point) : bool {
                return (Math.pow(point.x - roundRectangle.radius, 2) + Math.pow(point.y - roundRectangle.radius, 2)) < Math.pow(roundRectangle.radius, 2)
            }
        }

        HoverHandler {
            id: hoverHandler

            enabled: container.hoverEnabled

            onHoveredChanged: {
                if (container.hoverHandler && typeof container.hoverHandler === "function")
                    container.hovered = container.hoverHandler(container, hovered)
            }
        }

        TapHandler {
            enabled: container.clickEnabled

            onTapped: {
                if (container.clickHandler && typeof container.clickHandler === "function")
                    container.clickHandler(index)
            }
        }

        DragHandler {
            enabled: container.dragEnabled
            target: container

            onGrabChanged: (transition, point) => {
                if (container.dragHandler && typeof container.dragHandler === "function")
                    container.dragHandler(container, transition, point)
            }
        }

        WheelHandler {
            id: wheelHandler
            enabled: container.wheelEnabled
            target: container

            onWheel: event => {
                if (container.wheelHandler && typeof container.wheelHandler === "function")
                    container.wheelHandler(container, event, wheelHandler)
            }
        }

        PinchHandler {
            enabled: container.pinchEnabled
            target: container
        }
    }
}
