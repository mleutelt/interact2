import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

import Box2D

Page {
    id: container

    Component {
        id: circleComponent

        PCircle {
            world: level.physicsWorld

            item.color: Qt.rgba(Math.random(), Math.random(), Math.random())
        }
    }


    Component {
        id: rectangleComponent

        PRectangle {
            world: level.physicsWorld

            item.color: Qt.rgba(Math.random(), Math.random(), Math.random())
        }
    }

    component PopupMenu: Popup {
        id: popupItem
        required property var model

        signal buttonClicked(int value)

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
                    text: modelData

                    onClicked: {
                        popupItem.buttonClicked(modelData)
                        popupItem.close()
                    }
                }
            }
        }
    }

    Level {
        id: level
    }

    MouseArea {
        id: drawingMouseArea

        anchors.fill: parent

        onPressed: {
            if (App.editor.currentEditOperation === Editor.EditOperationType_Draw) {
                switch (App.editor.currentShape) {
                case Editor.ShapeType_Circle:
                case Editor.ShapeType_Rectangle:
                    drawingRectangle.initialX = mouseX
                    drawingRectangle.initialY = mouseY
                    break
                }
            }
        }
        onReleased: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle:
                circleComponent.createObject(level, {
                                                 width: Math.min(drawingRectangle.width, drawingRectangle.height),
                                                 height: Math.min(drawingRectangle.height, drawingRectangle.width),
                                                 x: drawingRectangle.x,
                                                 y: drawingRectangle.y,
                                                 bodyType: objectBehaviorButton.checked ? Body.Static : Body.Dynamic
                                             })
                break
            case Editor.ShapeType_Rectangle:
                rectangleComponent.createObject(level, {
                                                 width: drawingRectangle.width,
                                                 height: drawingRectangle.height,
                                                 x: drawingRectangle.x,
                                                 y: drawingRectangle.y,
                                                 bodyType: objectBehaviorButton.checked ? Body.Static : Body.Dynamic
                                             })
                break
            }
        }
    }

    Rectangle {
        id: drawingRectangle

        property int initialX: 0
        property int initialY: 0

        color: "red"
        opacity: 0.3
        visible: drawingMouseArea.pressed
        width: Math.abs(drawingMouseArea.mouseX - initialX)
        height: Math.abs(drawingMouseArea.mouseY - initialY)
        border.width: 1
        border.color: "black"
        x: Math.min(initialX, drawingMouseArea.mouseX)
        y: Math.min(initialY, drawingMouseArea.mouseY)
    }

    // TODO: figure out editor controls
    // - element selection (which physical element should be placed when clicking)
    // - element modification (delete etc.)
    // - save level to file
    // - load existing level from file
    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10

        RoundButton {
            id: shapesMenuButton

            checkable: true
            text: App.editor.currentShape

            onToggled: shapesMenu.open()

            PopupMenu {
                id: shapesMenu

                model: App.editor.availableShapes

                onButtonClicked: value => App.editor.currentShape = value
                onClosed: shapesMenuButton.toggle()
            }
        }

        RoundButton {
            id: editOperationButton

            checkable: true
            text: App.editor.currentEditOperation

            onToggled: editMenu.open()

            PopupMenu {
                id: editMenu

                model: App.editor.availableEditOperations

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

            onClicked: Screens.showPrevious()
        }
    }

    Dialog {
        id: levelPropertiesDialog

        title: "Level Properties"
        anchors.centerIn: parent
        width: parent.width / 2
        height: parent.height / 2
        modal: true

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
