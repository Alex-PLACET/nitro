#pragma once

#include "QtNodes/internal/ConvertersRegister.hpp"

#include <QtNodes/ConvertersRegister>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <nitro/datatypes/grayimagedata.hpp>


namespace nitro::imcore {

[[nodiscard]] ColImageData ColImageData_from_DecimalData(const DecimalData &decimal_data);

[[nodiscard]] ColImageData ColImageData_from_IntegerData(const IntegerData &integer_data);

[[nodiscard]] ColImageData ColImageData_from_GrayImageData(const GrayImageData &grayimage_data);

[[nodiscard]] QtNodes::ConvertersRegister Get_ColImageData_conversions();

}; // namespace nitro::imcore
