#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

class PsnrOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImProc
