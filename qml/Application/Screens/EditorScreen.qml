import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

import Box2D

Page {
    id: container

    Component {
        id: ball

        PCircle {
            world: level.physicsWorld
            bodyType: Body.Dynamic

            item.color: Qt.rgba(Math.random(), Math.random(), Math.random())
        }
    }

    Level {
        id: level
    }

    MouseArea {
        id: drawingMouseArea

        property point currentPosition: Qt.point(0, 0)

        anchors.fill: parent

        onPressed: {
            if (App.editor.currentEditOperation === Editor.EditOperationType_Draw) {
                switch (App.editor.currentShape) {
                case Editor.ShapeType_Circle:
                case Editor.ShapeType_Rectangle:
                    drawingRectangle.x = mouseX
                    drawingRectangle.y = mouseY
                    break
                }
            }
        }
        onPositionChanged: currentPosition = Qt.point(mouse.x, mouse.y)
        onReleased: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle: {
                var newBall = ball.createObject(level)
                newBall.width = Math.min(drawingRectangle.width, drawingRectangle.height)
                newBall.height = Math.min(drawingRectangle.height, drawingRectangle.width)
                newBall.x = drawingRectangle.x
                newBall.y = drawingRectangle.y
            }
            }

            currentPosition = Qt.point(0, 0)
        }
    }

    Rectangle {
        id: drawingRectangle
        color: "red"
        opacity: 0.3
        visible: drawingMouseArea.pressed
        width: drawingMouseArea.currentPosition.x - x
        height: drawingMouseArea.currentPosition.y - y
        border.width: 1
        border.color: "black"
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

            Popup {
                id: shapesMenu

                modal: true
                dim: false
                x: -width - shapesMenuLayout.spacing
                padding: 0

                RowLayout {
                    id: shapesMenuLayout
                    Repeater {
                        model: App.editor.availableShapes

                        RoundButton {
                            text: modelData

                            onClicked: {
                                App.editor.currentShape = modelData
                                shapesMenu.close()
                            }
                        }
                    }
                }

                onClosed: shapesMenuButton.toggle()
            }
        }

        RoundButton {
            id: editOperationButton

            checkable: true
            text: App.editor.currentEditOperation

            onToggled: editMenu.open()

            Popup {
                id: editMenu

                modal: true
                dim: false
                x: -width - editMenuLayout.spacing
                padding: 0

                RowLayout {
                    id: editMenuLayout
                    Repeater {
                        model: App.editor.availableEditOperations

                        RoundButton {
                            text: modelData

                            onClicked: {
                                App.editor.currentEditOperation = modelData
                                editMenu.close()
                            }
                        }
                    }
                }

                onClosed: editOperationButton.toggle()
            }
        }

        RoundButton {
            id: levelPropertiesButton

            checkable: true
            text: "props"

            onToggled: levelPropertiesDialog.open()
        }

        RoundButton {
            id: saveButton

            text: "save"

            onClicked: {
                if (levelNameInput.text) {
                    App.editor.saveLevel(levelNameInput.text)
                } else {
                    // TODO: show popup about missing name
                }
            }
        }

        Item {
            Layout.fillHeight: true
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
