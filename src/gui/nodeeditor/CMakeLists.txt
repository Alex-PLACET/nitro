file(GLOB_RECURSE CPPS */*.cpp)
file(GLOB_RECURSE HPPS */*.hpp)

add_library(nodeeditor STATIC
    ${CPPS}
    ${HPPS})

target_include_directories(nodeeditor
    PUBLIC
        include)

target_link_libraries(nodeeditor
    PUBLIC
        Qt6::Widgets
        QtNodes
        nodes
    PRIVATE
        components
        util
        version)