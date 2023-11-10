#include "nitro/core/nodes/datatypes/jsondata.hpp"

#include <QJsonDocument>

namespace nitro {

JsonData::JsonData() : FlexibleData<QJsonObject, JsonData>({}, id_, name_, baseColor_) {}

JsonData::JsonData(const QJsonObject &value)
    : FlexibleData<QJsonObject, JsonData>(value, id_, name_, baseColor_) {}

QString JsonData::getDescription() const {
    return QJsonDocument(data()).toJson();
}

void JsonData::registerConversions() {
    // Every type needs a "conversion" to itself
    JsonData::registerConversionFrom<JsonData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                return std::static_pointer_cast<JsonData>(nodeData)->data();
            });
}

} // namespace nitro
