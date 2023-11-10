#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class RandomOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImCore
