#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Compression {

class JpegOperator : public NodeOperator {
public:
    explicit JpegOperator(QLabel *valueLabel, QLabel *originalSizeLabel, QLabel *ratioLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
    QLabel *originalSizeLabel_;
    QLabel *ratioLabel_;
};

} // namespace nitro::Compression
