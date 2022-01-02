import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

import Box2D

Page {
    id: container

    component PopupMenu: Popup {
        id: popupItem

        required property var model
        required property var iconResolver

        signal buttonClicked(value: int)

        modal: true
        dim: false
        x: -width - shapesMenuLayout.spacing
        padding: 0

        RowLayout {
            id: shapesMenuLayout

            Repeater {
                id: repeater

                model: popupItem.model

                RoundButton {
                    font.family: Style.fontAwesomeRegular.font.family
                    font.weight: Font.Regular
                    text: popupItem.iconResolver(modelData)

                    onClicked: {
                        popupItem.buttonClicked(modelData)
                        popupItem.close()
                    }
                }
            }
        }
    }

    MouseArea {
        id: drawingMouseArea

        property size minimalObjectSize: Qt.size(10, 10)

        anchors.fill: parent

        onPressed: {
            if (App.editor.currentEditOperation === Editor.EditOperationType_Draw) {
                switch (App.editor.currentShape) {
                case Editor.ShapeType_Circle:
                case Editor.ShapeType_Rectangle:
                    drawingRectangle.initialX = mouseX
                    drawingRectangle.initialY = mouseY
                    break
                case Editor.ShapeType_Polygon:
                    canvas.color = Qt.rgba(Math.random(), Math.random(), Math.random())
                    canvas.currentX = mouseX
                    canvas.currentY = mouseY
                    break
                }
            }
        }
        onReleased: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle:
            case Editor.ShapeType_Rectangle:
                switch (App.editor.currentEditOperation) {
                case Editor.EditOperationType_Draw:
                    if (drawingRectangle.width > minimalObjectSize.width && drawingRectangle.height > minimalObjectSize.height) {
                        App.editor.addObject(App.editor.currentShape,
                                             Qt.rect(drawingRectangle.x, drawingRectangle.y, drawingRectangle.width, drawingRectangle.height),
                                             objectBehaviorButton.checked)
                    }

                    break
                case Editor.EditOperationType_Delete:
                case Editor.EditOperationType_Move:
                    break
                }
                break
            case Editor.ShapeType_Polygon:
                canvas.clear()
                // TODO: add object to level
                break
            }
        }
        onPositionChanged: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle:
            case Editor.ShapeType_Rectangle:
                break
            case Editor.ShapeType_Polygon:
                canvas.requestPaint()
                break
            }
        }
    }

    Level {
        id: level

        objectFactory.model: App.editor.levelData
        objectFactory.interactionHandler: function(index) {
            switch (App.editor.currentEditOperation) {
            case Editor.EditOperationType_Delete:
                App.editor.removeObject(index)
                break
            case Editor.EditOperationType_Draw:
            case Editor.EditOperationType_Move:
                break
            }
        }
    }

    Canvas {
        id: canvas

        property real currentX
        property real currentY
        property color color: Qt.rgba(Math.random(), Math.random(), Math.random())

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
        id: drawingRectangle

        property int initialX: 0
        property int initialY: 0

        color: "red"
        opacity: 0.3
        visible: drawingMouseArea.pressed &&
                 App.editor.currentEditOperation === Editor.EditOperationType_Draw &&
                 App.editor.currentShape !== Editor.ShapeType_Polygon
        width: Math.abs(drawingMouseArea.mouseX - initialX)
        height: Math.abs(drawingMouseArea.mouseY - initialY)
        border.width: 1
        border.color: "black"
        x: Math.min(initialX, drawingMouseArea.mouseX)
        y: Math.min(initialY, drawingMouseArea.mouseY)
    }

    // TODO: figure out editor controls
    // - load existing level from file
    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10

        RoundButton {
            id: shapesMenuButton

            checkable: true
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: Style.shapeIconForType(App.editor.currentShape)

            onToggled: shapesMenu.open()

            PopupMenu {
                id: shapesMenu

                model: App.editor.availableShapes
                iconResolver: Style.shapeIconForType

                onButtonClicked: value => App.editor.currentShape = value
                onClosed: shapesMenuButton.toggle()
            }
        }

        RoundButton {
            id: editOperationButton

            checkable: true
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: Style.editIconForType(App.editor.currentEditOperation)

            onToggled: editMenu.open()

            PopupMenu {
                id: editMenu

                model: App.editor.availableEditOperations
                iconResolver: Style.editIconForType

                onButtonClicked: value => App.editor.currentEditOperation = value
                onClosed: editOperationButton.toggle()
            }
        }


        RoundButton {
            id: objectBehaviorButton

            checkable: true
            text: checked ? "static" : "dynamic"
        }

        RoundButton {
            id: levelPropertiesButton

            checkable: true
            text: "props"

            onToggled: levelPropertiesDialog.open()
        }

        Item {
            Layout.fillHeight: true
        }

        RoundButton {
            id: saveButton

            text: "save"

            onClicked: {
                if (levelNameInput.text) {
                    level.grabToImage(result => App.editor.saveLevel(levelNameInput.text, result))
                } else {
                    // TODO: show popup about missing name
                }
            }
        }

        RoundButton {
            text: "done"

            onClicked: {
                App.editor.reset()
                Screens.showPrevious()
            }
        }
    }

    Dialog {
        id: levelPropertiesDialog

        title: "Level Properties"
        anchors.centerIn: parent
        width: parent.width / 2
        height: parent.height / 2
        modal: true

        // TODO: add background image selection
        // TODO: add music selection
        GridLayout {
            anchors.fill: parent
            columns: 2

            Label {
                Layout.fillWidth: true
                text: "Name"
            }

            TextField {
                id: levelNameInput

                Layout.fillWidth: true
            }

            Label {
                Layout.fillWidth: true
                text: "Gravity X"
            }

            SpinBox {
                id: gravityXSpinBox

                from: -10
                to: 10
                value: level.physicsWorld.gravity.x
            }

            Label {
                Layout.fillWidth: true
                text: "Gravity Y"
            }

            SpinBox {
                id: gravityYSpinBox

                from: -10
                to: 10
                value: level.physicsWorld.gravity.y
                editable: true
            }

            Item { Layout.fillHeight: true }
            Item { Layout.fillHeight: true }
        }

        onClosed: levelPropertiesButton.toggle()
    }
}
