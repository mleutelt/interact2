import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: polygonShape

    property real density: 1
    property real friction: 0.5
    property real restitution: 0.1

    // NOTE: initial position is determined by the bounding box
    x: model.boundingBox.x
    y: model.boundingBox.y

    Instantiator {
        model: points
        delegate: B2D.Polygon {
            density: container.density
            friction: container.friction
            restitution: container.restitution
            vertices: modelData

            onBeginContact: other => container.beginContact(other)
            onEndContact: other => container.endContact(other)
        }

        onObjectAdded: (index, object) => container.body.addFixture(object)
    }

    visualItem: Shape {
        id: polygonShape

        // NOTE: we can't use anchors here because we need to map from the
        // drawing canvas coordinate system, which is the whole screen, to
        // our local coordinate system.
        // Seems to be a bit off sometimes, but accurate enough.
        width: parent.width
        height: parent.height
        x: -model.boundingBox.x
        y: -model.boundingBox.y
        antialiasing: true
        containsMode: Shape.FillContains

        ShapePath {
            id: shapePath

            fillColor: container.itemColor
            fillRule: ShapePath.WindingFill
            strokeColor: container.hovered ? "red" : fillColor
            strokeWidth: container.hovered ? 2 : 1

            PathPolyline {
                id: pathPolyLine

                path: model.polygon
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
