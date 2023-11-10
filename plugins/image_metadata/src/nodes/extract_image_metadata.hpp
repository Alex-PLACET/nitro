#pragma once

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace custom {

class ImageMetadata : public nitro::NodeOperator {
public:
    static std::function<std::unique_ptr<nitro::NitroNode>()> creator(const QString &category);

    void execute(nitro::NodePorts &nodePorts) override;
};

} // namespace custom
