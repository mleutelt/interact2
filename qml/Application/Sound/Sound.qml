pragma Singleton

import QtQuick
import QtMultimedia

QtObject {
    function playMusic(title = "") {
        if (title) {
            backgroundMusic.source = "qrc:/music/%1".arg(title)
        }

        backgroundMusic.play()
    }

    function stopMusic() {
        backgroundMusic.pause()
    }

    property bool musicPlaying: backgroundMusic.playbackState === MediaPlayer.PlayingState
    property alias musicVolume: output.volume
    property MediaPlayer backgroundMusic: MediaPlayer {
        loops: MediaPlayer.Infinite
        audioOutput: AudioOutput {
            id: output
        }
    }
}
