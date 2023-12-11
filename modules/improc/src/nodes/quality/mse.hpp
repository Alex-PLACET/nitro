#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImProc {

class MseOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImProc
