#include "image_metadata_plugin.hpp"

#include "nodes/extract_image_metadata.hpp"

QtNodes::ConvertersRegister ImageMetadataPlugin::registerDataTypes() {
    return {};
}

std::vector<nitro::CreatorVariant> ImageMetadataPlugin::registerNodes() {
    std::vector<nitro::CreatorVariant> node_creators;
    static const QString category = "Image Metadata";
    node_creators.emplace_back(custom::ImageMetadata::creator(category));
    return node_creators;
}
