#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <opencv2/core/mat.hpp>


namespace nitro::ImCore {

class MixOperator : public NodeOperator {
public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    cv::Mat fac_;
    cv::Mat in1_;
    cv::Mat in2_;

    void initUnifiedInputs(const NodePorts &nodePorts);
};

} // namespace nitro::ImCore
