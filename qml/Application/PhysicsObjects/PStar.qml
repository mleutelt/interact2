import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: star

    property alias density: polygon.density
    property alias friction: polygon.friction
    property alias restitution: polygon.restitution
    property alias sensor: polygon.sensor
    property alias categories: polygon.categories
    property alias collidesWith: polygon.collidesWith
    property alias groupIndex: polygon.groupIndex

    // FIXME: vertices are not accurate
    physicalObject: [
        B2D.Polygon {
            id: polygon

            density: 1
            friction: 0.5
            restitution: 0.1
            vertices: [
                Qt.point(shapePath.startX, shapePath.startY),
                Qt.point(p1.x, p1.y),
                Qt.point(star.width * 2 / 5, p2.y),
            ]

            onBeginContact: other => container.beginContact(other)
            onEndContact: other => container.endContact(other)
        },
        B2D.Polygon {
            density: polygon.density
            friction: polygon.friction
            restitution: polygon.restitution
            vertices: [
                Qt.point(p2.x, p2.y),
                Qt.point(p3.x, p3.y),
                Qt.point(shapePath.startX, star.height * 4 / 5),
            ]

            onBeginContact: other => container.beginContact(other)
            onEndContact: other => container.endContact(other)
        },
        B2D.Polygon {
            density: polygon.density
            friction: polygon.friction
            restitution: polygon.restitution
            vertices: [
                Qt.point(p4.x, p4.y),
                Qt.point(shapePath.startX, shapePath.startY),
                Qt.point(star.width * 3 / 5, p2.y),
            ]

            onBeginContact: other => container.beginContact(other)
            onEndContact: other => container.endContact(other)
        }
    ]

    visualItem: Shape {
        id: star

        anchors.fill: parent
        antialiasing: true
        containsMode: Shape.FillContains

        ShapePath {
            id: shapePath

            fillColor: "gold"
            fillRule: ShapePath.WindingFill
            strokeColor: container.hovered ? "red" : fillColor
            strokeWidth: container.hovered ? 2 : 1
            startX: star.width / 2
            startY: 0

            PathLine { id: p1; x: star.width * 4 / 5; y: star.height }
            PathLine { id: p2; x: 0; y: star.height * 2 / 5 }
            PathLine { id: p3; x: star.width; y: star.height * 2 / 5 }
            PathLine { id: p4; x: star.width / 5; y: star.height }
            PathLine { id: p5; x: star.width / 2; y: 0 }
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
