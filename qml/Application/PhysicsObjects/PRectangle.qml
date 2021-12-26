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
    }
    visualItem: Rectangle {
        id: rectangle

        anchors.fill: parent
        color: Qt.rgba(Math.random(), Math.random(), Math.random())
    }
}
