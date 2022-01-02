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
        density: 1
        friction: 0.5
        restitution: 0.1

        onBeginContact: other => container.beginContact(other)
        onEndContact: other => container.endContact(other)
    }
    visualItem: Rectangle {
        id: rectangle

        anchors.fill: parent
        antialiasing: true
        color: Qt.rgba(Math.random(), Math.random(), Math.random())
    }

    Canvas {
        id: canvas
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        property real lastX
        property real lastY
        property color color: Qt.rgba(Math.random(), Math.random(), Math.random())

        onPaint: {
            var ctx = getContext('2d')
            ctx.lineWidth = 1.5
            ctx.strokeStyle = canvas.color
            ctx.beginPath()
            ctx.moveTo(lastX, lastY)
            lastX = area.mouseX
            lastY = area.mouseY
            ctx.lineTo(lastX, lastY)
            ctx.stroke()
        }
        MouseArea {
            id: area
            anchors.fill: parent
            onPressed: {
                canvas.color = Qt.rgba(Math.random(), Math.random(), Math.random())
                canvas.lastX = mouseX
                canvas.lastY = mouseY
            }
            onPositionChanged: {
                canvas.requestPaint()
            }
        }
    }
}

