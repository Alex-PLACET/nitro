#include <cmath>
#include <nitro/core/nodes/datatypes/integerdata_conversions.hpp>

namespace nitro {

IntegerData IntegerData_from_DecimalData(const DecimalData &decimal_data) {
    return IntegerData{static_cast<int>(std::round(decimal_data.data()))};
}
} // namespace nitro