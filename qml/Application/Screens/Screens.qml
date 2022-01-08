pragma Singleton

import QtQuick

// NOTE: central management of all screens the user can navigate between
QtObject {
    enum Screen {
        Main,
        Editor,
        LevelSelection,
        CurrentLevel
    }

    property int currentScreen: -1
    property int previousScreen: -1

    function file(screen) {
        switch (screen) {
            case Screens.Main: return "qrc:/App/qml/Application/Screens/MainScreen.qml"
            case Screens.Editor: return "qrc:/App/qml/Application/Screens/EditorScreen.qml"
            case Screens.LevelSelection: return "qrc:/App/qml/Application/Screens/LevelSelectionScreen.qml"
            case Screens.CurrentLevel: return "qrc:/App/qml/Application/Screens/CurrentLevelScreen.qml"
        }
    }

    function show(screen: int, properties) {
        previousScreen = currentScreen
        currentScreen = screen

        showScreen(screen, properties)
    }

    function showPrevious() {
        currentScreen = previousScreen

        showPreviousScreen()
    }

    signal showScreen(screen: int, properties: var)
    signal showPreviousScreen()
    signal showInitialScreen()
}
