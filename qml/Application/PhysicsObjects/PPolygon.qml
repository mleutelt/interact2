import QtQuick
import QtQuick.Shapes

import Box2D as B2D

PObject {
    id: container

    property alias item: rectangle

    property alias density: polygon.density
    property alias friction: polygon.friction
    property alias restitution: polygon.restitution
    property alias sensor: polygon.sensor
    property alias categories: polygon.categories
    property alias collidesWith: polygon.collidesWith
    property alias groupIndex: polygon.groupIndex

    // TODO: model this correctly
    physicalObject: B2D.Polygon {
        id: polygon

        density: 1
        friction: 0.5
        restitution: 0.1

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
    }
    // TODO: use QtQuick.Shapes (or Canvas) to draw the graphical representation
    visualItem: Rectangle {
        id: rectangle

        anchors.fill: parent
        antialiasing: true
        color: Qt.rgba(Math.random(), Math.random(), Math.random())
    }
}

