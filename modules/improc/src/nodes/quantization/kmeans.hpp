#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <opencv2/core/mat.hpp>


namespace nitro::ImProc {

class KMeansOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

cv::Mat kMeansHist(const cv::Mat &image, int numColors, int maxIter);

} // namespace nitro::ImProc
