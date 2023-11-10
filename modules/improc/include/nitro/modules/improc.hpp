#pragma once

#include "nitro/core/modules/nitromodule.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/core/nodes/noderegistry.hpp>

namespace nitro::ImProc {

class ImageViewer;

/**
 * @brief Describes the ImProc module. This module contains a number of nodes used for image processing, ranging from various filters to restoration and quantization nodes.
 */
class ImProc : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    ImProc();

    /**
     * @inherit
     */
    std::vector<CreatorVariant> registerNodes() override;

    /**
     * @inherit
     */
    void registerDataTypes() override{
            // there is no data type in this module
    };

private:
    void registerFilterNodes(std::vector<CreatorVariant> &node_creators) const;

    void registerAnalysisNodes(std::vector<CreatorVariant> &node_creators) const;

    void registerInputNodes(std::vector<CreatorVariant> &node_creators) const;

    void registerQualityMetricNodes(std::vector<CreatorVariant> &node_creators) const;

    void registerRestorationNodes(std::vector<CreatorVariant> &node_creators) const;

    void registerQuantizationNodes(std::vector<CreatorVariant> &node_creators) const;
};

} // namespace nitro::ImProc
