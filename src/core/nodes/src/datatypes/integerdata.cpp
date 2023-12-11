#include "nitro/core/nodes/datatypes/integerdata.hpp"


namespace nitro {

IntegerData::IntegerData() : FlexibleData<int, IntegerData>(0, id_, name_, baseColor_) {
}

IntegerData::IntegerData(int value)
    : FlexibleData<int, IntegerData>(value, id_, name_, baseColor_) {
}

QString IntegerData::getDescription() const {
    return QString::number(data());
}

} // namespace nitro
