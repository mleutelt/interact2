pragma Singleton

import QtQuick

import App

QtObject {
    id: container

    readonly property QtObject application: QtObject {
        readonly property size minimumWindowSize: Qt.size(800, 600)
    }

    readonly property FontLoader fontAwesomeSolid: FontLoader {
        source: "qrc:/fonts/FontAwesome/Font Awesome 6 Free-Solid-900.otf"
    }

    function shapeIconForType(type) {
        switch (type) {
        case Constants.ShapeType_Circle: return "\uf111"
        case Constants.ShapeType_Rectangle: return "\uf0c8"
        case Constants.ShapeType_Polygon: return "\uf5ee"
        case Constants.ShapeType_SpecialStar: return "\uf005"
        }
    }

    function editIconForType(type) {
        switch (type) {
        case Editor.EditOperationType_Draw: return "\uf304"
        case Editor.EditOperationType_Move: return "\uf25a"
        case Editor.EditOperationType_Delete: return "\uf2ed"
        }
    }
}
