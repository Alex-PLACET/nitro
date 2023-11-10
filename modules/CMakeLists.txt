# Include the custom module functions
include(../cmake/modules.cmake)

add_module(ImCore)
add_module(ImProc)
add_module(Compression)
#add_module(Thesis)

# Adds and links the modules
# Remove any modules added more than once
list(REMOVE_DUPLICATES ENABLED_MODULES)

message(STATUS "Enabled modules: ${ENABLED_MODULES}")
# Generate file that automatically registers all enabled modules
foreach (module ${ENABLED_MODULES})
    # Set up code to generate in config.hpp.in
    # The name of the module's header file is assumed to be identical to the name of the module (lowercase).
    # e.g. the module ImCore expects a header file called imcore.hpp.
    # The modules are expected to reside in the modules/ directory and the module directory itself should also be the lowercase version of the module name.
    string(TOLOWER ${module} lModule)
    set(MODULE_INCLUDES "${MODULE_INCLUDES}#include \"nitro/modules/${lModule}.hpp\"\n")
    # Add the module at runtime
    set(MODULE_APPENDS "${MODULE_APPENDS}modules.push_back(std::make_unique<nitro::${module}::${module}>());\n\t")

    # Ensure we call the CMakeLists.txt of the corresponding module
    add_subdirectory(${PROJECT_SOURCE_DIR}/modules/${lModule})
endforeach ()

# Create the modules_list.hpp file containing the runtime code to register the modules
message(STATUS "Generating module_list.hpp")
configure_file(include/nitro/modules/module_list.hpp.in ${CMAKE_BINARY_DIR}/include/nitro/modules/module_list.hpp  @ONLY)

add_library(module_list STATIC ${CMAKE_BINARY_DIR}/include/nitro/modules/module_list.hpp)
target_include_directories(module_list PUBLIC ${CMAKE_BINARY_DIR}/include)
target_link_libraries(module_list
    PUBLIC
    module_interface
    ImCore
    ImProc
    Compression)
