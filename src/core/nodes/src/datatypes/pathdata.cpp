#include "nitro/core/nodes/datatypes/pathdata.hpp"

namespace nitro {

PathData::PathData()
    : FlexibleData<std::filesystem::path, PathData>(std::filesystem::path(),
                                                    id_,
                                                    name_,
                                                    baseColor_) {}

PathData::PathData(const std::filesystem::path &value)
    : FlexibleData<std::filesystem::path, PathData>(value, id_, name_, baseColor_) {}

QString PathData::getDescription() const {
    return QString::fromStdWString(data().wstring());
}

void PathData::registerConversions() {

    // Every type needs a "conversion" to itself
    PathData::registerConversionFrom<PathData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                return std::static_pointer_cast<PathData>(nodeData)->data();
            });
}

} // namespace nitro
