set(MODULE_NAME Compression)

find_package(ZLIB REQUIRED)

file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)
add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

target_link_libraries(${MODULE_NAME}
    PUBLIC
    module_interface
    nodes
    PRIVATE
    nodeeditor
    ImCore
    ImProc
    ZLIB::ZLIB
    opencv_imgcodecs
)

target_include_directories(${MODULE_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${MODULE_NAME} PUBLIC resources/resources.qrc)
