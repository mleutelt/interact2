import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects

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

        radius: fakeCircle.radius
        density: 1
        friction: 0.5
        restitution: 0.3

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
    }
    visualItem: Item {
        id: roundRectangle

        anchors.fill: parent
        containmentMask: QtObject {
            function contains(point: point) : bool {
                return (Math.pow(point.x - fakeCircle.radius, 2) + Math.pow(point.y - fakeCircle.radius, 2)) < Math.pow(fakeCircle.radius, 2)
            }
        }

        Image {
            id: image
            anchors.fill: parent
            antialiasing: true
            source: "qrc:/images/paper3.png"
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mask
                cached: true
            }
        }

        Rectangle {
            id: mask
            anchors.fill: parent
            radius: container.width / 2
            color: Qt.alpha(container.itemColor, 1)
            visible: false
        }

        Rectangle {
            id: fakeCircle
            anchors.fill: parent
            antialiasing: true
            radius: container.width / 2
            color: Qt.alpha(container.itemColor, 0.5)
            border.color: container.hovered ? "red" : "transparent"
            border.width: container.hovered ? 2 : 0
        }

        RadialGradient {
            source: fakeCircle
            cached: true
            visible: container.isGameItem
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#00000000" }
                GradientStop { position: 1.0; color: "#FF000000" }
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
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            onTapped: (eventPoint, button) => {
                if (container.clickHandler && typeof container.clickHandler === "function")
                    container.clickHandler(container, index, button)
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
