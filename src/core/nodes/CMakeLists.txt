file(GLOB_RECURSE CPPS */*.cpp)
file(GLOB_RECURSE HPPS */*.hpp)

add_library(nodes STATIC
    ${CPPS}
    ${HPPS})

target_include_directories(nodes
    PUBLIC
        include)

target_link_libraries(nodes
    PUBLIC
        Qt6::Gui
        Qt6::Core
        QtNodes
        valueSliders
    PRIVATE
        util)