import QtQuick
import QtQuick.Controls

import App

ApplicationWindow {
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
}
