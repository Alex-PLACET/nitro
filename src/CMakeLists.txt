add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(util)

qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    main.cpp
    ../resources/resources.qrc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    QtNodes
    nodeeditor
    module_list
    version
    nodes
)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )