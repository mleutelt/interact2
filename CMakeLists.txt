cmake_minimum_required(VERSION 3.16)

project(Interact2 VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(qml_plugins_deployment_dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/qml)

find_package(Qt6 6.2
    COMPONENTS
        Quick
        Widgets
        Multimedia
        ShaderTools
    REQUIRED
)

qt_add_resources(RESOURCES
    resources/resources.qrc
    resources/resources_media.qrc
)

# NOTE: for some reason qt_add_big_resources causes compile errors on iOS,
# so we're deploying bigger files manually
set(music_files
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/music/Lullaby.mp3
)

if(IOS)
    qt_add_executable(${PROJECT_NAME}
        main.cpp
        ${RESOURCES}
        ${music_files}
    )

    # NOTE: on iOS we're linking the plugin statically into the app
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Quick
        Qt6::Widgets
        Qt6::Multimedia
        box2dplugin
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "${music_files}"
    )
else()
    qt_add_executable(${PROJECT_NAME}
        main.cpp
        ${RESOURCES}
    )

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Quick
        Qt6::Widgets
        Qt6::Multimedia
    )

    set(music_deployment_dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/music)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${music_deployment_dir}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${music_files} ${music_deployment_dir}
    )
endif()

qt_add_shaders(${PROJECT_NAME} "shaders"
    PREFIX "/"
    FILES
        resources/shaders/vertex.vert
        resources/shaders/fragment.frag
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
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
