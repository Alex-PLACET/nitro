#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class RgbOperator : public NodeOperator {

public:
    explicit RgbOperator(QLabel *colLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *colLabel_;
    QPixmap colLabelPixMap_;
};

} // namespace nitro::ImCore
