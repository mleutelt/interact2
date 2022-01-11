pragma Singleton

import QtQuick
import QtMultimedia

QtObject {
    function playMusic(title = "") {
        if (title) {
            backgroundMusic.source = title
        }

        backgroundMusic.play()
    }

    function stopMusic() {
        backgroundMusic.pause()
    }

    readonly property bool musicPlaying: backgroundMusic.playbackState === MediaPlayer.PlayingState
    property alias musicVolume: output.volume
    property MediaPlayer backgroundMusic: MediaPlayer {
        loops: MediaPlayer.Infinite
        audioOutput: AudioOutput {
            id: output
        }
    }
}
