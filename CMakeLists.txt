cmake_minimum_required(VERSION 3.16)

project(Interact2 VERSION 0.3 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(qml_plugins_deployment_dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/qml)

find_package(Qt6 6.2
    COMPONENTS
        Quick
        Multimedia
    REQUIRED
)

qt_add_big_resources(LARGE_RESOURCES
    resources/resources_media.qrc
)

qt_add_resources(RESOURCES resources/resources.qrc)

qt_add_executable(${PROJECT_NAME}
    main.cpp
    ${RESOURCES}
    ${LARGE_RESOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia
)

set_source_files_properties(
    qml/Application/Screens/Screens.qml
    qml/Application/Style/Style.qml
    qml/Application/Sound/Sound.qml
    PROPERTIES QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(${PROJECT_NAME}
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
        levelinteractor.h
        levelinteractor.cpp
        ilevelmanager.h
)

# TODO: make deployment to mobile devices work
#if(IOS)
#    message("iOS detected")
#qt_finalize_target(appInteract2)
#endif()

#if(MACOS)
#    message("macOS detected")
#    set_target_properties(${CUR_TARGET} PROPERTIES
#        BUNDLE True
#        MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${CUR_TARGET}
#        MACOSX_BUNDLE_BUNDLE_NAME ${CUR_TARGET}
#        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
#        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
#        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/customtemplate.plist.in
#    )
#endif()

add_subdirectory(3rdparty)
