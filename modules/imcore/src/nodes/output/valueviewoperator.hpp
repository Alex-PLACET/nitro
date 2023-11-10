#pragma once

#include "gui/imviewer/imageviewer.hpp"

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>


namespace nitro::ImCore {

class ValueViewOperator : public NodeOperator {
public:
    explicit ValueViewOperator(QLabel *valueLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
};

} // namespace nitro::ImCore
