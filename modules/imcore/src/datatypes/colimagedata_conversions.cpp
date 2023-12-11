#include "nitro/datatypes/colimagedata_conversions.hpp"

namespace nitro::imcore {

ColImageData ColImageData_from_DecimalData(const DecimalData &decimal_data) {
    const double val = decimal_data.data();
    return ColImageData{cv::Mat(1, 1, CV_32F, cv::Scalar(val, val, val))};
}

ColImageData ColImageData_from_IntegerData(const IntegerData &integer_data) {
    const double val = integer_data.data() / 255.0;
    return ColImageData{cv::Mat(1, 1, CV_32F, cv::Scalar(val, val, val))};
}

ColImageData ColImageData_from_GrayImageData(const GrayImageData &grayimage_data) {
    cv::Mat res;
    cv::cvtColor(*grayimage_data.data(), res, cv::COLOR_GRAY2RGB);
    return ColImageData{res};
}

QtNodes::ConvertersRegister Get_ColImageData_conversions() {
    QtNodes::ConvertersRegister converters_register;
    converters_register.add_converter(ColImageData_from_DecimalData);
    converters_register.add_converter(ColImageData_from_IntegerData);
    converters_register.add_converter(ColImageData_from_GrayImageData);
    return converters_register;
}
}; // namespace nitro::imcore
