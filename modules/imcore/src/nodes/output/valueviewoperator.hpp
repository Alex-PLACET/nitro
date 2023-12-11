#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class ValueViewOperator : public NodeOperator {
public:
    explicit ValueViewOperator(QLabel *valueLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
};

} // namespace nitro::ImCore
