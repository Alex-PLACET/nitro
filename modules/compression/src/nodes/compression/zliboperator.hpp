#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Compression {

class ZLibOperator : public NodeOperator {
public:
    explicit ZLibOperator(QLabel *valueLabel,
                          QLabel *originalSizeLabel,
                          QLabel *ratioLabel,
                          QLabel *timeLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
    QLabel *originalSizeLabel_;
    QLabel *ratioLabel_;
    QLabel *timeLabel_;
};

} // namespace nitro::Compression
