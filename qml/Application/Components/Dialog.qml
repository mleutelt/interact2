import QtQuick
import QtQuick.Controls as QQC
import QtQuick.Controls.impl
import Qt5Compat.GraphicalEffects

QQC.Dialog {
    id: container

    anchors.centerIn: QQC.ApplicationWindow.overlay

    QQC.Overlay.modal: Item {
        FastBlur {
            anchors.fill: parent
            source: QQC.ApplicationWindow.contentItem
            radius: 24
        }

        Rectangle {
            anchors.fill: parent
            color: Color.transparent(container.palette.shadow, 0.5)
        }
    }

    QQC.Overlay.modeless: Item {
        FastBlur {
            anchors.fill: parent
            source: QQC.ApplicationWindow.contentItem
            radius: 24
        }

        Rectangle {
            anchors.fill: parent
            color: Color.transparent(container.palette.shadow, 0.12)
        }
    }
}
