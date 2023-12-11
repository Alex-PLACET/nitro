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

} // namespace nitro
