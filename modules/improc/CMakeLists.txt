set(MODULE_NAME ImProc)

requires_module(ImCore)

file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)

# remove file containing "flip" in the path
list(FILTER MODULE_SOURCES EXCLUDE REGEX ".*flip.*")
list(FILTER MODULE_HEADERS EXCLUDE REGEX ".*flip.*")

add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

target_link_libraries(${MODULE_NAME}
    PUBLIC
    nodes
    ImCore
    PRIVATE
    nodeeditor
    opencv_photo
    util)

target_include_directories(${MODULE_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${MODULE_NAME} PUBLIC resources/resources.qrc)
