#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class LayerRemovalOperator : public NodeOperator {
public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::Thesis
