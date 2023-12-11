#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

class InvertOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImCore
