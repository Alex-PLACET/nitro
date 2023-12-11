#include "extract_image_metadata.hpp"

#include <QString>
#include <QtNodes/ConvertersRegister>
#include <exiv2/exiv2.hpp>
#include <nitro/core/nodes/datatypes/jsondata.hpp>
#include <nitro/core/nodes/datatypes/pathdata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <qjsonobject.h>
#include <qjsonvalue.h>

namespace custom {

static const QString INPUT_PATH = QStringLiteral("Path");
static const QString OUTPUT_JSON = QStringLiteral("JSON");

QJsonValue Exiv2ValueToQJsonValue(const Exiv2::Value &value) {
    switch (value.typeId()) {
        case Exiv2::unsignedByte:
            return QJsonValue(static_cast<qint64>(value.toUint32()));
        case Exiv2::asciiString:
            return QJsonValue(QString::fromStdString(value.toString()));
        case Exiv2::unsignedShort:
            return QJsonValue(static_cast<int>(value.toUint32()));
        case Exiv2::unsignedLong:
            return QJsonValue(value.toInt64());
        case Exiv2::unsignedRational:
            return QJsonValue(value.toString().c_str());
        case Exiv2::signedByte:
            return QJsonValue(value.toInt64());
        case Exiv2::undefined:
            return QJsonValue(value.toString().c_str());
        case Exiv2::signedShort:
            return QJsonValue(value.toInt64());
        case Exiv2::signedLong:
            return QJsonValue(value.toInt64());
        case Exiv2::signedRational:
            return QJsonValue(value.toString().c_str());
        case Exiv2::tiffFloat:
            return QJsonValue(value.toFloat());
        case Exiv2::tiffDouble:
            return QJsonValue(value.toString().c_str());
        case Exiv2::tiffIfd:
            return QJsonValue(value.toString().c_str());
        case Exiv2::unsignedLongLong:
            return QJsonValue(value.toString().c_str());
        case Exiv2::signedLongLong:
            return QJsonValue(value.toInt64());
        case Exiv2::tiffIfd8:
        case Exiv2::string:
        case Exiv2::date:
        case Exiv2::time:
        case Exiv2::comment:
        case Exiv2::directory:
        case Exiv2::xmpText:
        case Exiv2::xmpAlt:
        case Exiv2::xmpBag:
        case Exiv2::xmpSeq:
        case Exiv2::langAlt:
        case Exiv2::invalidTypeId:
        case Exiv2::lastTypeId:
            return QJsonValue(value.toString().c_str());
    }
}

template<typename T>
QJsonObject Exiv2DataToQJsonObject(const T &data) {
    QJsonObject json;

    for (auto it = data.begin(); it != data.end(); ++it) {
        QString familyName = QString::fromStdString(it->familyName());
        QString groupName = QString::fromStdString(it->groupName());
        QString tagName = QString::fromStdString(it->tagName());

        // Retrieve or create the family object
        QJsonObject familyObject = json.value(familyName).toObject();

        // Retrieve or create the group object within the family object
        QJsonObject groupObject = familyObject.value(groupName).toObject();

        // Set the tag value
        QJsonValue value = Exiv2ValueToQJsonValue(it->value());
        groupObject.insert(tagName, value);

        // Update the family object with the modified group object
        familyObject.insert(groupName, groupObject);

        // Update the main JSON object with the modified family object
        json.insert(familyName, familyObject);
    }
    return json;
}

void ImageMetadata::execute(nitro::NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const std::filesystem::path path = nodePorts.inGetAs<nitro::PathData>(INPUT_PATH);
    if (path.empty()) {
        return;
    }
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Path does not exist");
    }
    const Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(path.string());
    image->readMetadata();
    const Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        // TODO: log warning
    }
    QJsonObject exif_data_json = Exiv2DataToQJsonObject(exifData);

    const Exiv2::XmpData &xmp_data = image->xmpData();

    if (xmp_data.empty()) {
        //  TODO: Log warning
    }

    QJsonObject xmp_data_json = Exiv2DataToQJsonObject(xmp_data);

    QJsonObject metadata_json;
    metadata_json.insert("exif", exif_data_json);
    metadata_json.insert("xmp", xmp_data_json);

    nodePorts.output<nitro::JsonData>(OUTPUT_JSON, metadata_json);
}

nitro::CreatorWithoutParameters ImageMetadata::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        nitro::NitroNodeBuilder builder("Image metadata", "image_metadata", category);
        return builder.withOperator(std::make_unique<ImageMetadata>())
                ->withIcon("todo-txt.svg")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withInputPort<nitro::PathData>(INPUT_PATH)
                ->withOutputPort<nitro::JsonData>(OUTPUT_JSON)
                ->build(converters_register);
    };
}

} // namespace custom
