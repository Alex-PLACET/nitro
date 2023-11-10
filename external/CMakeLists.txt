set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_CLANG_TIDY "")

# QtNodes
add_subdirectory(nodeeditor)


# FLIP
option(FLIP_LIBRARY ON)
add_subdirectory(flip)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/flip/cpp/CPP)
list(APPEND NITRO_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/flip/cpp/common)

# QT Value Sliders
option(SLIDER_LIBRARY ON)
add_subdirectory(qt-value-slider)
