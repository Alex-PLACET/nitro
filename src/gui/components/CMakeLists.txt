file(GLOB_RECURSE CPPS */*.cpp)
file(GLOB_RECURSE HPPS */*.hpp)

add_library(components STATIC
    ${CPPS}
    ${HPPS})

target_include_directories(components
    PUBLIC
        include)

target_link_libraries(components
    PUBLIC
        Qt6::Widgets)