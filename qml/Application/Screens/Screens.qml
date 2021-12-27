pragma Singleton

import QtQuick

// NOTE: central management of all screens the user can navigate between
QtObject {
    enum Screen {
        Main,
        Editor,
        Settings,
        LevelSelection,
        CurrentLevel
    }

    function file(screen) {
        switch (screen) {
            case Screens.Main: return "qrc:/App/qml/Application/Screens/MainScreen.qml"
            case Screens.Editor: return "qrc:/App/qml/Application/Screens/EditorScreen.qml"
            case Screens.Settings: return "qrc:/App/qml/Application/Screens/SettingsScreen.qml"
            case Screens.LevelSelection: return "qrc:/App/qml/Application/Screens/LevelSelectionScreen.qml"
            case Screens.CurrentLevel: return "qrc:/App/qml/Application/Screens/CurrentLevelScreen.qml"
        }
    }

    signal show(int screen)
    signal showPrevious()
    signal showInitial()
}
