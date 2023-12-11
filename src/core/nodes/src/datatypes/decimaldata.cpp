#include "nitro/core/nodes/datatypes/decimaldata.hpp"

namespace nitro {

DecimalData::DecimalData() : FlexibleData<double, DecimalData>(0, id_, name_, baseColor_) {}

DecimalData::DecimalData(double value)
    : FlexibleData<double, DecimalData>(value, id_, name_, baseColor_) {}

QString DecimalData::getDescription() const {
    return QString::number(data(), 'f', 3);
}

} // namespace nitro
