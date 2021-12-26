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
    }
    visualItem: Rectangle {
        id: roundRectangle

        anchors.fill: parent
        radius: container.width / 2
        color: "black"
    }
}
