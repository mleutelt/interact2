import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: polygonShape

    property real density: 1
    property real friction: 0.5
    property real restitution: 0.1

    // NOTE: initial position is the starting point of the polyline
    x: pathPolyLine.start.x
    y: pathPolyLine.start.y

    Instantiator {
        model: points
        delegate: B2D.Polygon {
            density: container.density
            friction: container.friction
            restitution: container.restitution
            vertices: modelData

            onBeginContact: other => container.beginContact(other)
            onEndContact: other => container.endContact(other)

            Component.onCompleted: console.log("poly fixture created from points:", modelData)
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

        ShapePath {
            id: shapePath

            fillColor: container.itemColor
            strokeColor: fillColor
            fillRule: ShapePath.WindingFill

            PathPolyline {
                id: pathPolyLine

                path: model.polygon
            }
        }
    }
}
