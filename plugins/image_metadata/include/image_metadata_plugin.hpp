#pragma once

#include <nitro/core/modules/nitromodule.hpp>

class ImageMetadataPlugin : public QObject, public nitro::NitroModule {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.nitro.NitroModule" FILE "image_metadata_plugin.json")
    Q_INTERFACES(nitro::NitroModule)
public:
    ImageMetadataPlugin() = default;

    /**
     * @inherit
     */
    void registerDataTypes() override;

    /**
     * @inherit
     */
    std::vector<nitro::CreatorVariant> registerNodes() override;
};
