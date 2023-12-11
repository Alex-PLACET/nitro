#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>


namespace nitro::ImCore {

class MapRangeOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImCore
