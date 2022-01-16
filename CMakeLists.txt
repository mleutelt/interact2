cmake_minimum_required(VERSION 3.16)

project(Interact2 VERSION 0.3 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2
    COMPONENTS
        Quick
        Multimedia
    REQUIRED
)

qt_add_big_resources(LARGE_RESOURCES
    resources/resources.qrc
)

qt_add_executable(appInteract2
    main.cpp
    ${LARGE_RESOURCES}
)

set_source_files_properties(
    qml/Application/Screens/Screens.qml
    qml/Application/Style/Style.qml
    qml/Application/Sound/Sound.qml
    PROPERTIES QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(appInteract2
    URI App
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/Application/Components/ImageButton.qml
        qml/Application/Components/Dialog.qml
        qml/Application/Components/Button.qml
        qml/Application/Components/RoundButton.qml
        qml/Application/Sound/Sound.qml
        qml/Application/Screens/Screens.qml
        qml/Application/Screens/MainScreen.qml
        qml/Application/Screens/EditorScreen.qml
        qml/Application/Screens/LevelSelectionScreen.qml
        qml/Application/Screens/CurrentLevelScreen.qml
        qml/Application/Levels/Level.qml
        # FIXME: ideally the physics objects should be put into another importable module
        # to allow namespaced imports in qml
        qml/Application/PhysicsObjects/PObject.qml
        qml/Application/PhysicsObjects/PRectangle.qml
        qml/Application/PhysicsObjects/PPolygon.qml
        qml/Application/PhysicsObjects/PLine.qml
        qml/Application/PhysicsObjects/PCircle.qml
        qml/Application/PhysicsObjects/PObjectFactory.qml
        qml/Application/PhysicsObjects/PStar.qml
        qml/Application/Style/Style.qml
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
        leveldata.h
        levelfileio.h
        levelfileio.cpp
        physicsobjectoptimizer.h
        physicsobjectoptimizer.cpp
        concavechopper.h
        concavechopper.cpp
        constants.h
)

target_compile_definitions(appInteract2
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

target_link_libraries(appInteract2
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia
)
