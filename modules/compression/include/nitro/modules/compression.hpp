#pragma once

#include "QtNodes/internal/ConvertersRegister.hpp"
#include "nitro/core/modules/nitromodule.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/core/nodes/noderegistry.hpp>


namespace nitro::Compression {

/**
 * @brief Describes the Compression module. Contains numerous nodes that allow for the compression of images.
 */
class Compression : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    Compression();

    /**
     * @inherit
     */
    QtNodes::ConvertersRegister registerDataTypes() override { return {}; };

    /**
     * @inherit
     */
    std::vector<CreatorVariant> registerNodes() override;

private:
    void registerCompressionNodes(std::vector<CreatorVariant> &registry) const;
};

} // namespace nitro::Compression
