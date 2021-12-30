import QtQuick
import QtQuick.Controls

import App

ApplicationWindow {
    id: applicationWindow

    width: 640
    height: 480
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainScreen {}
    }

    Connections {
        target: Screens

        function onShow(screen) {
            stackView.push(Screens.file(screen))
        }

        function onShowPrevious() {
            stackView.pop()
        }

        function onShowInitial() {
            stackView.pop(null)
        }
    }

    Item {
        id: pointerArea

        anchors.fill: parent

        PointHandler {
            id: handler
            target: Rectangle {
                parent: pointerArea
                visible: handler.active
                color: "transparent"
                width: 30
                height: width
                radius: width / 2
                border.width: 1
                border.color: "black"
                x: handler.point.position.x - width / 2
                y: handler.point.position.y - height / 2
            }
        }
    }
}
