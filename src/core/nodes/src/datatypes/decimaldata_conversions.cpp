#include "QtNodes/internal/ConvertersRegister.hpp"

#include <nitro/core/nodes/datatypes/decimaldata_conversions.hpp>

namespace nitro {
DecimalData DecimalData_from_IntegerData(const IntegerData &integer_data) {
    return DecimalData{static_cast<double>(integer_data.data())};
}

QtNodes::ConvertersRegister Get_DecimalData_conversions() {
    QtNodes::ConvertersRegister converters_register;
    converters_register.add_converter(DecimalData_from_IntegerData);
    return converters_register;
}

} // namespace nitro