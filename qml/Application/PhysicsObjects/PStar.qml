import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects

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

    isGameItem: true
    isGoalItem: true
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

    visualItem: Item {
        anchors.fill: parent

        Image {
            id: image
            anchors.fill: parent
            antialiasing: true
            source: "qrc:/images/paper1.png"
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mask
                cached: true
            }
        }

        Shape {
            id: mask

            anchors.fill: parent
            visible: false

            ShapePath {
                fillColor: Qt.alpha("gold", 1)
                fillRule: ShapePath.WindingFill
                strokeWidth: 0
                startX: mask.width / 2
                startY: 0

                PathLine { id: p1; x: mask.width * 4 / 5; y: mask.height }
                PathLine { id: p2; x: 0; y: mask.height * 2 / 5 }
                PathLine { id: p3; x: mask.width; y: mask.height * 2 / 5 }
                PathLine { id: p4; x: mask.width / 5; y: mask.height }
                PathLine { id: p5; x: mask.width / 2; y: 0 }
            }
        }

        Shape {
            id: star

            anchors.fill: parent
            antialiasing: true
            containsMode: Shape.FillContains

            ShapePath {
                id: shapePath

                fillColor: Qt.alpha("gold", 0.5)
                fillRule: ShapePath.WindingFill
                strokeColor: container.hovered ? "red" : fillColor
                strokeWidth: container.hovered ? 2 : 0
                startX: star.width / 2
                startY: 0

                PathLine { x: star.width * 4 / 5; y: star.height }
                PathLine { x: 0; y: star.height * 2 / 5 }
                PathLine { x: star.width; y: star.height * 2 / 5 }
                PathLine { x: star.width / 5; y: star.height }
                PathLine { x: star.width / 2; y: 0 }
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
}
