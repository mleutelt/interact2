import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: polygonShape

    property alias density: polygon.density
    property alias friction: polygon.friction
    property alias restitution: polygon.restitution
    property alias sensor: polygon.sensor
    property alias categories: polygon.categories
    property alias collidesWith: polygon.collidesWith
    property alias groupIndex: polygon.groupIndex

    // NOTE: initial position is the starting point of the polyline
    x: pathPolyLine.start.x
    y: pathPolyLine.start.y

    // TODO: model this correctly
    // This propably needs to be an array of triangle vertices, that has been created
    // by the concave chopper
//    physicalObject: B2D.Polygon {
//        id: polygon

//        density: 1
//        friction: 0.5
//        restitution: 0.1

//        onBeginContact: other => container.beginContact(other)
//        onEndContact: other => container.endContact(other)
//    }
    // NOTE: only here to have valid physical object
    physicalObject: B2D.Box {
        id: polygon

        width: container.width
        height: container.height
        density: 1
        friction: 0.5
        restitution: 0.1

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
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
