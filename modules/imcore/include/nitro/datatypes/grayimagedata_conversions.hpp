#pragma once

#include <QtNodes/ConvertersRegister>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <nitro/datatypes/grayimagedata.hpp>

namespace nitro::imcore {

[[nodiscard]] GrayImageData GrayImageData_from_DecimalData(const DecimalData &decimal_data);

[[nodiscard]] GrayImageData GrayImageData_from_IntegerData(const IntegerData &integer_data);

[[nodiscard]] GrayImageData GrayImageData_from_ColImageData(const ColImageData &grayimage_data);

[[nodiscard]] QtNodes::ConvertersRegister Get_GrayImageData_conversions();

}; // namespace nitro::imcore
