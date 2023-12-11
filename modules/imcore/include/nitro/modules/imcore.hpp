#pragma once

#include "QtNodes/internal/ConvertersRegister.hpp"

#include <QtNodes/ConvertersRegister>
#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/core/nodes/noderegistry.hpp>

namespace nitro::ImCore {
class ImageViewer;
class HistogramView;

/**
 * @brief Describes the ImCore module. This module contains a number of nodes relevant for manipulating images and numbers. Also adds support for the image data types and image viewers.
 * Any module relying on images should require this module.
 */
class ImCore : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    ImCore() = default;

    /**
     * @brief Registers the data types for grayscale and color images.
     */
    QtNodes::ConvertersRegister registerDataTypes() override;

    /**
     * @inherit
     */
    std::vector<CreatorVariant> registerNodes() override;

private:
    static void registerInputNodes(std::vector<CreatorVariant> &node_creators);

    static void registerOutputNodes(std::vector<CreatorVariant> &node_creators);

    static void registerConvertNodes(std::vector<CreatorVariant> &node_creators);

    static void registerColorNodes(std::vector<CreatorVariant> &node_creators);

    static void registerTransformNodes(std::vector<CreatorVariant> &node_creators);
};

} // namespace nitro::ImCore
