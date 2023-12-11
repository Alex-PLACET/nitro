#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace custom {

class ImageMetadata : public nitro::NodeOperator {
public:
    static nitro::CreatorWithoutParameters creator(const QString &category);

    void execute(nitro::NodePorts &nodePorts) override;
};

} // namespace custom
