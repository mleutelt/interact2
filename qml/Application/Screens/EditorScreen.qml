import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.folderlistmodel

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

    component SelectionMenu: Popup {
        id: selectionMenu

        property alias model: gridView.model
        property alias delegate: gridView.delegate

        parent: Overlay.overlay
        anchors.centerIn: parent
        modal: true
        contentWidth: parent.width * 3 / 4
        contentHeight: parent.height * 3 / 4

        GridView {
            id: gridView

            anchors.fill: parent
            cellWidth: gridView.width / 2
            cellHeight: cellWidth
            clip: true
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
                case Editor.ShapeType_SpecialStar:
                    drawingRectangle.initialX = mouseX
                    drawingRectangle.initialY = mouseY
                    break
                case Editor.ShapeType_Polygon:
                    canvas.color = Qt.rgba(Math.random(), Math.random(), Math.random())
                    canvas.currentX = mouseX
                    canvas.currentY = mouseY
                    physicsObjectOptimizer.clear();
                    break
                }
            }
        }
        onReleased: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle:
            case Editor.ShapeType_Rectangle:
            case Editor.ShapeType_SpecialStar:
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
                physicsObjectOptimizer.idealizeLine();
                if (physicsObjectOptimizer.isClosed())
                {
                    // TODO: create Polygon
                }
                else
                {
                    // TODO: create line
                }
                // TODO: add object to level
                break
            }
        }
        onPositionChanged: {
            switch (App.editor.currentShape) {
            case Editor.ShapeType_Circle:
            case Editor.ShapeType_Rectangle:
            case Editor.ShapeType_SpecialStar:
                break
            case Editor.ShapeType_Polygon:
                if (drawingMouseArea.pressedButtons & Qt.LeftButton)
                {
                    physicsObjectOptimizer.addVertex(mouseX, mouseY);
                }
                canvas.requestPaint()
                break
            }
        }
    }

    Level {
        id: level

        backgroundImage.source: App.editor.levelData.backgroundImage
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

    ColumnLayout {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 10

        RoundButton {
            id: openLevelButton

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Open existing levels")
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: "\uf07c"

            onClicked: levelSelectionDialog.open()
        }

        RoundButton {
            id: saveButton

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Save the level")
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: "\uf0c7"

            onClicked: {
                if (levelNameInput.text) {
                    level.grabToImage(result => App.editor.saveLevel(levelNameInput.text, result))
                } else {
                    // TODO: show popup about missing name
                }
            }
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10

        RoundButton {
            id: shapesMenuButton

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Shape selection")
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

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Editing")
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

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Static/dynamic object mode")
            checkable: true
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: checked ? "\uf78c" : "\uf31e"
        }

        RoundButton {
            id: levelPropertiesButton

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Level properties")
            font.family: Style.fontAwesomeRegular.font.family
            font.weight: Font.Regular
            text: "\uf1de"

            onClicked: levelPropertiesDialog.open()
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Dialog {
        id: levelPropertiesDialog

        parent: Overlay.overlay
        anchors.centerIn: parent
        modal: true

        // TODO: add music selection
        GridLayout {
            id: propertiesGrid

            anchors.fill: parent
            columns: 2

            Label {
                Layout.fillWidth: true
                text: qsTr("Name")
            }

            TextField {
                id: levelNameInput

                Layout.fillWidth: true
                text: App.editor.levelData.name
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Gravity X")
            }

            SpinBox {
                id: gravityXSpinBox

                Layout.alignment: Qt.AlignRight
                from: -10
                to: 10
                value: level.physicsWorld.gravity.x
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Gravity Y")
            }

            SpinBox {
                id: gravityYSpinBox

                Layout.alignment: Qt.AlignRight
                from: -10
                to: 10
                value: level.physicsWorld.gravity.y
                editable: true
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Background")
            }

            ImageButton {
                id: backgroundSelection

                Layout.fillWidth: true
                Layout.maximumWidth: propertiesGrid.width / 2
                Layout.fillHeight: true
                Layout.preferredHeight: Layout.maximumWidth
                Layout.alignment: Qt.AlignRight
                image: App.editor.levelData.backgroundImage
                text: qsTr("Select image")

                onClicked: backgroundSelectionPopup.open()

                SelectionMenu {
                    id: backgroundSelectionPopup
                    model: FolderListModel {
                        folder: "qrc:/backgrounds"
                    }
                    delegate: ImageButton {
                        implicitWidth: GridView.view.cellWidth
                        implicitHeight: GridView.view.cellHeight
                        image: model.fileUrl
                        text: model.fileBaseName

                        onClicked: {
                            App.editor.levelData.backgroundImage = model.fileUrl
                            backgroundSelection.text = model.fileBaseName
                            backgroundSelectionPopup.close()
                        }
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Music")
            }

            Button {
                id: musicSelection

                Layout.fillWidth: true
                Layout.maximumWidth: propertiesGrid.width / 2
                Layout.alignment: Qt.AlignRight
                text: qsTr("Select music")

                onClicked: musicSelectionPopup.open()

                SelectionMenu {
                    id: musicSelectionPopup
                    model: FolderListModel {
                        folder: "qrc:/music"
                    }
                    delegate: Button {
                        implicitWidth: GridView.view.cellWidth
                        implicitHeight: GridView.view.cellHeight
                        text: model.fileBaseName

                        onClicked: {
                            // TODO: store music in level data
                            musicSelection.text = model.fileBaseName
                            musicSelectionPopup.close()
                        }
                    }
                }
            }


            Item { Layout.fillHeight: true }
            Item { Layout.fillHeight: true }
        }
    }

    Dialog {
        id: levelSelectionDialog

        parent: Overlay.overlay
        anchors.centerIn: parent
        contentWidth: parent.width * 3 / 4
        contentHeight: parent.height * 3 / 4
        modal: true
        padding: 0

        LevelSelectionScreen {
            anchors.fill: parent

            levelSelectionHandler: function(path) {
                App.editor.loadLevel(path)
                levelSelectionDialog.close()
            }
        }
    }

    StackView.onDeactivated: App.editor.reset()
}
