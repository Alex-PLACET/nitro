#pragma once

#include <QtPlugin>
#include <functional>
#include <variant>
#include <vector>

namespace nitro {

class MainWindow;

class NitroNode;

using CreatorWithoutParameters = std::function<std::unique_ptr<NitroNode>()>;
using CreatorWithWindow = std::function<CreatorWithoutParameters(MainWindow *window)>;
using CreatorVariant = std::variant<CreatorWithWindow, CreatorWithoutParameters>;

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
     * @brief Used to register new data types. If amodule introduces new data types, this function should be implemented.
     * Note that the conversion registration is done via the static methods in the corresponding data classes.
     */
    virtual void registerDataTypes() = 0;
};

} // namespace nitro

// NOLINTBEGIN
#define NitroModule_iid "org.nitro.NitroModule"          // NOLINT
Q_DECLARE_INTERFACE(nitro::NitroModule, NitroModule_iid) // NOLINT
// NOLINTEND
