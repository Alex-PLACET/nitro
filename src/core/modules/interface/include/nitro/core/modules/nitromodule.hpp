#pragma once

#include <QtNodes/ConvertersRegister>
#include <QtPlugin>
#include <nitro/core/nodes/node_creator.hpp>
#include <variant>
#include <vector>

namespace nitro {

/**
 * @brief Defines a generic module interface that any newly created module should inherit from.
 * Nodes and data types can be added by registering them to the NodeRegistry.
 */
class NitroModule {
public:
    /**
     * @brief Default destructor.
     */
    virtual ~NitroModule() = default;

    /**
     * @brief Used to register new nodes to the existing registry. If a module introduces new nodes, this function should be implemented.
     */
    virtual std::vector<CreatorVariant> registerNodes() = 0;

    /**
     * @brief Used to register new data types. If a module introduces new data types, this function should be implemented.
     * Note that the conversion registration is done via the static methods in the corresponding data classes.
     */
    virtual QtNodes::ConvertersRegister registerDataTypes() = 0;
};

} // namespace nitro

// NOLINTBEGIN
#define NitroModule_iid "org.nitro.NitroModule"          // NOLINT
Q_DECLARE_INTERFACE(nitro::NitroModule, NitroModule_iid) // NOLINT
// NOLINTEND
