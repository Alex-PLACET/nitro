#include "extract_image_metadata.hpp"

#include "nitro/core/nodes/datatypes/jsondata.hpp"

#include <QString>
#include <exiv2/exiv2.hpp>
#include <nitro/core/nodes/datatypes/pathdata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>

namespace custom {

static const QString INPUT_PATH = QStringLiteral("Path");
static const QString OUTPUT_JSON = QStringLiteral("JSON");

void ImageMetadata::execute(nitro::NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto path = nodePorts.inGetAs<nitro::PathData>(INPUT_PATH);
    const Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(path.string());
    image->readMetadata();
    const Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        // TODO: log warning
    }
    QJsonObject json;

    for (auto it = exifData.begin(); it != exifData.end(); ++it) {
        json.insert(QString::fromStdString(it->key()),
                    QString::fromStdString(it->value().toString()));
    }

    const Exiv2::XmpData &xmpData = image->xmpData();

    if (xmpData.empty()) {
        //  TODO: Log warning
    }
    for (auto it = xmpData.begin(); it != xmpData.end(); ++it) {
        json.insert(QString::fromStdString(it->key()),
                    QString::fromStdString(it->value().toString()));
    }

    nodePorts.output<nitro::JsonData>(OUTPUT_JSON, json);
}

std::function<std::unique_ptr<nitro::NitroNode>()> ImageMetadata::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Image metadata", "image_metadata", category);
        return builder.withOperator(std::make_unique<ImageMetadata>())
                ->withIcon("todo-txt.svg")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withInputPort<nitro::PathData>(INPUT_PATH)
                ->withOutputPort<nitro::JsonData>(OUTPUT_JSON)
                ->build();
    };
}

} // namespace custom
