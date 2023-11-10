set(MODULE_NAME ImCore)

# -------------------------------------------------------------------------- #
#                                Dependencies                                #
# -------------------------------------------------------------------------- #
requires_module(ImCore)

find_package(Qt6 REQUIRED COMPONENTS Charts)

# -------------------------------------------------------------------------- #
#                            Create module target                            #
# -------------------------------------------------------------------------- #
file(GLOB_RECURSE MODULE_SOURCES src/*.cpp)
file(GLOB_RECURSE MODULE_HEADERS src/*.hpp)
add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

target_link_libraries(${MODULE_NAME}
    PUBLIC
        nodes
        opencv_core
        module_interface
    PRIVATE
        nodeeditor
        components
        Qt6::Charts
        util)

# -------------------------------------------------------------------------- #
#                         Set up include directories                         #
# -------------------------------------------------------------------------- #
target_include_directories(${MODULE_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(${MODULE_NAME} PUBLIC resources/resources.qrc)
