import QtQuick
import QtQuick.Controls as QQC

import App

QQC.RoundButton {
    id: container

    font: Style.fontAwesomeSolid.font

    onClicked: Sound.playSound(Sound.Pop)
}
