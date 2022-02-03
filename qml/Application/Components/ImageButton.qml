import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import App

Button {
    id: container

    property alias image: image.source
    property alias loading: busyIndicator.running

    font.capitalization: Font.Capitalize

    contentItem: ColumnLayout {
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Material.color(Material.Grey)

            Image {
                id: image

                anchors.fill: parent
                anchors.margins: 1
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                smooth: true

                BusyIndicator {
                    id: busyIndicator

                    anchors.centerIn: parent
                    running: false
                }
            }
        }

        IconLabel {
            Layout.fillWidth: true
            Layout.leftMargin: 40
            Layout.rightMargin: 40
            Layout.minimumHeight: 24
            alignment: Qt.AlignVCenter
            font: container.font
            text: container.text
        }
    }

    onClicked: Sound.playSound(Sound.Pop)
}
