cmake_minimum_required(VERSION 3.27)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0144 NEW)

set(CMAKE_CXX_STANDARD 23)

# -------------------------------------------------------------------------- #
#                                 Versioning                                 #
# -------------------------------------------------------------------------- #

file(STRINGS "version.txt" NITRO_VERSION LIMIT_COUNT 1)
set(NITRO_NAME "NITRO")

project(nitro VERSION ${NITRO_VERSION} LANGUAGES CXX)

# -------------------------------------------------------------------------- #
#                                   Options                                  #
# -------------------------------------------------------------------------- #
set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -------------------------------------------------------------------------- #
#                             Required components                            #
# -------------------------------------------------------------------------- #

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL OpenGLWidgets)
set(QT_DIR ${Qt6_DIR})
qt_standard_project_setup()
find_package(OpenCV REQUIRED)
# find_package(OpenMP)

# -------------------------------------------------------------------------- #
#                           3rd Party dependencies                           #
# -------------------------------------------------------------------------- #

add_subdirectory(external)

add_compile_definitions(
    # Prevents Windows.h from adding unnecessary includes
    WIN32_LEAN_AND_MEAN
    # Prevents Windows.h from defining min/max as macros 
    NOMINMAX)

# -------------------------------------------------------------------------- #
#                                   Modules                                  #
# -------------------------------------------------------------------------- #
add_subdirectory(src)

add_subdirectory(modules)
add_subdirectory(plugins)
