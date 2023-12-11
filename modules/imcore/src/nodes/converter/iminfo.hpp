#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>


namespace nitro::ImCore {

class ImInfoOperator : public NodeOperator {
public:
    ImInfoOperator(QLabel *typeLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *typeLabel_;
};

} // namespace nitro::ImCore
