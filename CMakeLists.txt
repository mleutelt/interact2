cmake_minimum_required(VERSION 3.16)

project(Interact2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

qt_add_executable(appInteract2
    main.cpp
)

set_source_files_properties(qml/Application/Screens/Screens.qml PROPERTIES
    QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(appInteract2
    URI App
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/Application/Screens/Screens.qml
        qml/Application/Screens/MainScreen.qml
        qml/Application/Screens/EditorScreen.qml
        qml/Application/Screens/LevelSelectionScreen.qml
        qml/Application/Screens/SettingsScreen.qml
        qml/Application/Screens/CurrentLevelScreen.qml
        qml/Application/Levels/Level.qml
        # FIXME: ideally the physics objects should be put into another importable module
        # to allow namespaced imports in qml
        qml/Application/PhysicsObjects/PObject.qml
        qml/Application/PhysicsObjects/PRectangle.qml
        qml/Application/PhysicsObjects/PCircle.qml
    SOURCES
        app.h
        app.cpp
        editor.h
        editor.cpp
        levelhandler.h
        levelhandler.cpp
        levelmodel.h
        levelmodel.cpp
        leveldatamodel.h
        leveldatamodel.cpp
    RESOURCES
        "resources/levels/Level 1/data.json"
        "resources/levels/Level 1/preview.png"
        "resources/levels/Level 2/data.json"
        "resources/levels/Level 2/preview.png"
        "resources/levels/Level 3/data.json"
        "resources/levels/Level 3/preview.png"
        "resources/levels/Level 4/data.json"
        "resources/levels/Level 4/preview.png"
        "resources/levels/Level 5/data.json"
        "resources/levels/Level 5/preview.png"
)

target_compile_definitions(appInteract2
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

target_link_libraries(appInteract2
    PRIVATE
    Qt6::Quick)
