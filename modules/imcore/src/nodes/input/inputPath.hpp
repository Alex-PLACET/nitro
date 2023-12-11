#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>


namespace nitro::ImCore {

class InputPath : public NodeOperator {
public:
    explicit InputPath(QLabel *displayPathLabel);

    static CreatorWithoutParameters creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *displayPathLabel_;
};

} // namespace nitro::ImCore
