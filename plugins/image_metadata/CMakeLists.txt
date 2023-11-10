set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package("exiv2")

file(GLOB_RECURSE PLUGIN_SOURCES src/*.cpp)
file(GLOB_RECURSE PLUGIN_HEADERS include/*.hpp)
file(GLOB_RECURSE RESOURCES resources/*.qrc)

qt_add_plugin(image_metadata SHARED
    CLASS_NAME ImageMetadataPlugin
    ${PLUGIN_SOURCES} ${PLUGIN_HEADERS} ${RESOURCES}
    include/image_metadata_plugin.json
    image_metadata_plugin.json)

target_link_libraries(image_metadata
    PRIVATE
    module_interface
    exiv2::exiv2
)

target_include_directories(image_metadata
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(image_metadata
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:nitro>"
    LIBRARY_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:nitro>"
)
