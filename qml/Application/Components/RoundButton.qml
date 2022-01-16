import QtQuick
import QtQuick.Controls as QQC

import App

QQC.RoundButton {
    id: container

    font.family: Style.fontAwesomeRegular.font.family
    font.weight: Font.Regular

    onClicked: Sound.playSound(Sound.Pop)
}
