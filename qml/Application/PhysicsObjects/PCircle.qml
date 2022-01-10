import QtQuick

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
    }
}
