file(GLOB_RECURSE CPPS */*.cpp)
file(GLOB_RECURSE HPPS */*.hpp)

add_library(util STATIC
    ${CPPS}
    ${HPPS})

target_include_directories(util
    PUBLIC
        include)

target_link_libraries(util
    PUBLIC
        Qt6::Gui
        opencv_core
        opencv_imgproc)