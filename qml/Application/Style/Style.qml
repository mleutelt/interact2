pragma Singleton

import QtQuick

import App

QtObject {
    id: container

    readonly property FontLoader fontAwesomeRegular: FontLoader {
        source: "qrc:/App/resources/fonts/FontAwesome/Font Awesome 6 Free-Regular-400.otf"
    }

    readonly property FontLoader fontAwesomeSolid: FontLoader {
        source: "qrc:/App/resources/fonts/FontAwesome/Font Awesome 6 Free-Solid-900.otf"
    }

    function shapeIconForType(type) {
        switch (type) {
        case Editor.ShapeType_Circle: return "\uf111"
        case Editor.ShapeType_Rectangle: return "\uf2fb"
        case Editor.ShapeType_Polygon: return "\uf5ee"
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
