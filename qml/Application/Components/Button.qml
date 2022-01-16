import QtQuick
import QtQuick.Controls as QQC

import App

QQC.Button {
    id: container

    font.capitalization: Font.Capitalize

    onClicked: Sound.playSound(Sound.Pop)
}
