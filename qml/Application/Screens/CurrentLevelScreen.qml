import QtQuick
import QtQuick.Controls

import App

Page {
    id: container

    MouseArea {
        id: drawingMouseArea

        property size minimalObjectSize: Qt.size(10, 10)
        property var pointBuffer: []
        property point initialPoint: Qt.point(0, 0)
        property int distance: 0

        anchors.fill: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        cursorShape: Qt.PointingHandCursor

        onPressed: {
            drawingMouseArea.initialPoint = Qt.point(mouseX, mouseY)
            canvas.currentX = mouseX
            canvas.currentY = mouseY
        }
        onReleased: (mouse) => {

            if (mouse.button === Qt.LeftButton) {
                canvas.clear()
                levelInteractor.addPolygonObject(Constants.ShapeType_Polygon,
                                                 PhysicsObjectOptimizer.determineAndOptimizeObject(pointBuffer))
                pointBuffer = []
            }

            if (mouse.button === Qt.RightButton && distance > minimalObjectSize.width) {
                levelInteractor.addSimpleObject(Constants.ShapeType_Circle, Qt.rect(drawingMouseArea.initialPoint.x - distance / 2,
                                                                                    drawingMouseArea.initialPoint.y - distance / 2,
                                                                                    distance,
                                                                                    distance))
                distance = 0
            }
        }
        onPositionChanged: {
            distance = Math.abs(initialPoint.x - mouseX) + Math.abs(initialPoint.y - mouseY)

            if (pressedButtons & Qt.LeftButton) {
                pointBuffer.push(Qt.point(mouseX, mouseY))
                canvas.requestPaint()
            }
        }
    }

    LevelInteractor {
        id: levelInteractor

        levelManager: App.levelHandler
    }

    Level {
        id: level

        backgroundImage.source: App.levelHandler.currentLevelData.backgroundImage
        objectFactory.model: App.levelHandler.currentLevelData
        objectFactory.clickEnabled: true
        objectFactory.clickHandler: (index, button) => {
            if (button === Qt.MiddleButton) {
                levelInteractor.removeObject(index)
            }
        }
        objectFactory.hoverEnabled: true
        objectFactory.hoverHandler: function(item, hovered) {
            return hovered
        }
    }

    Canvas {
        id: canvas

        property real currentX
        property real currentY
        property color color: Material.color(Material.Red)

        function clear() {
            context.reset()
            canvas.requestPaint()
        }

        anchors.fill: parent
        contextType: "2d"

        onPaint: {
            context.lineWidth = 4
            context.strokeStyle = canvas.color
            context.beginPath()
            context.moveTo(currentX, currentY)
            currentX = drawingMouseArea.mouseX
            currentY = drawingMouseArea.mouseY
            context.lineTo(currentX, currentY)
            context.stroke()
        }
    }

    Rectangle {
        x: drawingMouseArea.initialPoint.x - drawingMouseArea.distance / 2
        y: drawingMouseArea.initialPoint.y - drawingMouseArea.distance / 2
        width: drawingMouseArea.distance
        height: drawingMouseArea.distance
        radius: drawingMouseArea.distance / 2
        visible: drawingMouseArea.pressed && (drawingMouseArea.pressedButtons & Qt.RightButton)
        border.width: 1
        border.color: "black"
        color: "transparent"
    }
}
