pragma Singleton

import QtQuick
import QtQuick.Window

import App

QtObject {
    id: container

    readonly property QtObject platform: QtObject {
        readonly property bool mobile: Qt.platform.os === "android" || Qt.platform.os === "ios"
        readonly property bool desktop: !mobile
    }

    readonly property QtObject application: QtObject {
        readonly property int windowFlags: platform.mobile ? Qt.Window | Qt.MaximizeUsingFullscreenGeometryHint : Qt.Window
        readonly property int windowVisibility: platform.mobile ? Qt.WindowFullScreen : Qt.WindowMaximized
        readonly property size minimumWindowSize: platform.mobile ? Qt.size(Screen.width, Screen.height) : Qt.size(800, 600)
        readonly property size preferredWindowSize: minimumWindowSize
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
