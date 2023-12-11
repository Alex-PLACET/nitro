#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class ResampleOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

cv::Mat resampleImage(const cv::Mat &img, bool brightnessCorrect, double kSize = 0.125);

} // namespace nitro::Thesis
