pragma Singleton

import QtQml
import QtMultimedia

QtObject {
    id: container

    enum Sounds {
        Plopp,
        Pling,
        Pop,
        PaperCrumple,
        PaperSlide,
        Bounce,
        Bell
    }

    function playMusic(title = "") {
        if (title) {
            musicPlayer.source = title
        }

        musicPlayer.play()
    }

    function stopMusic() {
        musicPlayer.pause()
    }

    function playSound(sound: int) {
        if (!effectsEnabled)
            return

        var file

        switch (sound) {
        case Sound.Plopp:
            file = "qrc:/sounds/plopp.wav"
            break
        case Sound.Pling:
            file = "qrc:/sounds/kling.wav"
            break
        case Sound.Pop:
            file = "qrc:/sounds/pop.wav"
            break
        case Sound.PaperCrumple:
            file = "qrc:/sounds/paper-crumple.wav"
            break
        case Sound.PaperSlide:
            file = "qrc:/sounds/paper-slide.wav"
            break
        case Sound.Bounce:
            file = "qrc:/sounds/ball-bouncing.wav"
            break
        case Sound.Bell:
            file = "qrc:/sounds/attention-bell-ding.wav"
            break
        }

        soundEffectComponent.incubateObject(container, { source: file })
    }

    readonly property bool musicPlaying: musicPlayer.playbackState === MediaPlayer.PlayingState
    property alias musicVolume: output.volume

    property bool effectsEnabled: true
    property real effectsVolume: 1.0

    readonly property QtObject internal: QtObject {
        property MediaPlayer musicPlayer: MediaPlayer {
            id: musicPlayer

            loops: MediaPlayer.Infinite
            audioOutput: AudioOutput {
                id: output
            }
        }

        property Component soundEffectComponent: Component {
            id: soundEffectComponent

            SoundEffect {
                volume: container.effectsVolume
                onPlayingChanged: {
                    if (!playing) {
                        destroy()
                    }
                }

                Component.onCompleted: play()
            }
        }
    }
}
