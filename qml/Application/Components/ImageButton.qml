import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App

Button {
    id: container

    property alias image: image.source
    property alias loading: busyIndicator.running

    font.capitalization: Font.Capitalize

    contentItem: ColumnLayout {
        Image {
            id: image

            Layout.fillWidth: true
            Layout.fillHeight: true
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            smooth: true

            BusyIndicator {
                id: busyIndicator

                anchors.centerIn: parent
                running: false
            }
        }

        IconLabel {
            Layout.fillWidth: true
            font: container.font
            text: container.text
        }
    }

    onClicked: Sound.playSound(Sound.Pop)
}
