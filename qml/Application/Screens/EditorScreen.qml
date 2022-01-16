import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
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

        property alias view: gridView

        parent: Overlay.overlay
        anchors.centerIn: parent
        modal: true
        contentWidth: parent.width * 3 / 4
        contentHeight: parent.height * 3 / 4

        GridView {
            id: gridView

            anchors.fill: parent
            clip: true
        }
    }

    MouseArea {
        id: drawingMouseArea

        property size minimalObjectSize: Qt.size(10, 10)
        property var pointBuffer: []

        anchors.fill: parent

        onPressed: {
            console.log("Mouse button pressed at (" + mouseX + ", " + mouseY + ")")
            if (App.editor.currentEditOperation === Editor.EditOperationType_Draw) {
                switch (App.editor.currentShape) {
                case Constants.ShapeType_Circle:
                case Constants.ShapeType_Rectangle:
                case Constants.ShapeType_SpecialStar:
                    drawingRectangle.initialX = mouseX
                    drawingRectangle.initialY = mouseY
                    break
                case Constants.ShapeType_Polygon:
                    canvas.currentX = mouseX
                    canvas.currentY = mouseY
                    break
                }
            }
        }
        onReleased: {
            console.log("Mouse button released at (" + mouseX + ", " + mouseY + ")")
            switch (App.editor.currentShape) {
            case Constants.ShapeType_Circle:
            case Constants.ShapeType_Rectangle:
            case Constants.ShapeType_SpecialStar:
                switch (App.editor.currentEditOperation) {
                case Editor.EditOperationType_Draw:
                    if (drawingRectangle.width > minimalObjectSize.width && drawingRectangle.height > minimalObjectSize.height) {
                        App.editor.addSimpleObject(App.editor.currentShape,
                                                   Qt.rect(drawingRectangle.x, drawingRectangle.y, drawingRectangle.width, drawingRectangle.height),
                                                   objectBehaviorButton.checked)
                    }

                    break
                case Editor.EditOperationType_Delete:
                case Editor.EditOperationType_Move:
                    break
                }
                break
            case Constants.ShapeType_Polygon:
                canvas.clear()
                App.editor.addPolygonObject(App.editor.currentShape,
                                            PhysicsObjectOptimizer.determineAndOptimizeObject(pointBuffer),
                                            objectBehaviorButton.checked)
                pointBuffer = []
                break
            }
        }
        onPositionChanged: {
            switch (App.editor.currentShape) {
            case Constants.ShapeType_Circle:
            case Constants.ShapeType_Rectangle:
            case Constants.ShapeType_SpecialStar:
                break
            case Constants.ShapeType_Polygon:
                if (pressed) {
                    console.log("Pushing mouse coordinates (" + mouseX + ", " + mouseY + ")")
                    pointBuffer.push(Qt.point(mouseX, mouseY))
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
        id: drawingRectangle

        property int initialX: 0
        property int initialY: 0

        visible: drawingMouseArea.pressed &&
                 App.editor.currentEditOperation === Editor.EditOperationType_Draw &&
                 App.editor.currentShape !== Constants.ShapeType_Polygon
        width: Math.abs(drawingMouseArea.mouseX - initialX)
        height: Math.abs(drawingMouseArea.mouseY - initialY)
        x: Math.min(initialX, drawingMouseArea.mouseX)
        y: Math.min(initialY, drawingMouseArea.mouseY)
        color: Color.transparent("black", 0.1)
        border.width: 2
        border.color: Material.color(Material.Red)
        radius: 2
        layer.enabled: true
        layer.effect: ElevationEffect {
            elevation: 6
        }
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
                    view.cellWidth: view.width / 2
                    view.cellHeight: view.cellWidth
                    view.model: FolderListModel {
                        folder: "qrc:/backgrounds"
                    }
                    view.delegate: Item {
                        implicitWidth: GridView.view.cellWidth
                        implicitHeight: GridView.view.cellHeight

                        ImageButton {
                            anchors.fill: parent
                            anchors.margins: 10
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
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("Music")
            }

            Button {
                id: musicSelection

                property string selectedMusic

                Layout.fillWidth: true
                Layout.maximumWidth: propertiesGrid.width / 2
                Layout.alignment: Qt.AlignRight
                text: selectedMusic ? selectedMusic : qsTr("Select music")

                onClicked: musicSelectionPopup.open()

                SelectionMenu {
                    id: musicSelectionPopup
                    view.cellHeight: 48
                    view.cellWidth: view.width
                    view.model: FolderListModel {
                        folder: "qrc:/music"
                    }
                    view.delegate: CheckDelegate {
                        implicitWidth: GridView.view.cellWidth
                        implicitHeight: GridView.view.cellHeight
                        checked: musicSelection.text === model.fileBaseName
                        text: model.fileBaseName

                        onToggled: {
                            if (checked) {
                                App.editor.levelData.music = "qrc" + model.filePath
                                musicSelection.selectedMusic = model.fileBaseName
                            } else {
                                App.editor.levelData.music = ""
                                musicSelection.selectedMusic = ""
                            }
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
        topPadding: 0

        LevelSelectionScreen {
            anchors.fill: parent

            levelSelectionHandler: function(path) {
                App.editor.loadLevel(path)
                levelSelectionDialog.close()
            }
        }
    }

    Connections {
        target: App.editor

        function onLevelLoadedSuccessfully() {
            Sound.playMusic(App.editor.levelData.music)
        }
    }

    StackView.onDeactivated: App.editor.reset()
}
