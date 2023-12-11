#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class CompressOperator : public NodeOperator {
public:
    explicit CompressOperator(QLabel *timeLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *timeLabel_;
};

} // namespace nitro::Thesis
