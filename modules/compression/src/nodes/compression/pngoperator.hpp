#pragma once

#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Compression {

class PngOperator : public NodeOperator {
public:
    explicit PngOperator(QLabel *valueLabel, QLabel *originalSizeLabel, QLabel *ratioLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
    QLabel *originalSizeLabel_;
    QLabel *ratioLabel_;
};

} // namespace nitro::Compression
