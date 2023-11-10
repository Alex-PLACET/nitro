add_library(module_interface INTERFACE)

target_link_libraries(module_interface
    INTERFACE
    QtNodes
    nodes
)

target_include_directories(module_interface
    INTERFACE
        include)
