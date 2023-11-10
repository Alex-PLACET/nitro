message(STATUS "Configuring version header")
configure_file(include/nitro/version.hpp.in ${CMAKE_BINARY_DIR}/include/nitro/version.hpp @ONLY)
add_library(version INTERFACE ${CMAKE_BINARY_DIR}/include/nitro/version.hpp)
target_include_directories(version INTERFACE ${CMAKE_BINARY_DIR}/include)
