#pragma once

#include "QtNodes/internal/ConvertersRegister.hpp"

#include <QtNodes/ConvertersRegister>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>

namespace nitro {
IntegerData IntegerData_from_DecimalData(const DecimalData &decimal_data);

QtNodes::ConvertersRegister Get_IntegerData_conversions();
} // namespace nitro