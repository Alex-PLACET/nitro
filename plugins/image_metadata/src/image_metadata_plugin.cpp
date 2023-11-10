#include "image_metadata_plugin.hpp"

#include "nodes/extract_image_metadata.hpp"

void ImageMetadataPlugin::registerDataTypes() {
    // No data types to register
}

std::vector<CreatorVariant> ImageMetadataPlugin::registerNodes() {
    std::vector<CreatorVariant> node_creators;
    static const QString category = "Image Metadata";
    node_creators.emplace_back(custom::ImageMetadata::creator(category));
    return node_creators;
}
