import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: container

    property alias image: image.source
    property alias loading: busyIndicator.running

    contentItem: ColumnLayout {
        Image {
            id: image

            Layout.fillWidth: true
            Layout.fillHeight: true
            fillMode: Image.PreserveAspectCrop

            BusyIndicator {
                id: busyIndicator

                anchors.centerIn: parent
                running: false
            }
        }
        Label {
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
            text: container.text
        }
    }
}
