#pragma once

#include "QtNodes/internal/ConvertersRegister.hpp"

#include <QtNodes/ConvertersRegister>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>

namespace nitro {

DecimalData DecimalData_from_IntegerData(const IntegerData &integer_data);

QtNodes::ConvertersRegister Get_DecimalData_conversions();

} // namespace nitro