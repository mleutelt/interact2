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
        border.color: container.hovered ? "red" : "transparent"
        border.width: container.hovered ? 2 : 0

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
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            onTapped: (eventPoint, button) => {
                if (container.clickHandler && typeof container.clickHandler === "function")
                    container.clickHandler(index, button)
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
