#include <nitro/datatypes/grayimagedata_conversions.hpp>

namespace nitro::imcore {

GrayImageData GrayImageData_from_DecimalData(const DecimalData &decimal_data) {
    const double val = decimal_data.data();
    return GrayImageData{std::make_shared<cv::Mat>(1, 1, CV_32F, cv::Scalar(val))};
}

GrayImageData GrayImageData_from_IntegerData(const IntegerData &integer_data) {
    const double val = integer_data.data() / 255.0;
    return GrayImageData{cv::Mat(1, 1, CV_32F, cv::Scalar(val, val, val))};
}

GrayImageData GrayImageData_from_ColImageData(const ColImageData &colimage_data) {
    if (colimage_data.data()->channels() == 1) {
        return GrayImageData{std::make_shared<cv::Mat>(*colimage_data.data())};
    }
    cv::Mat res;
    cv::cvtColor(*colimage_data.data(), res, cv::COLOR_RGB2GRAY);
    res.convertTo(res, CV_32FC1);
    return GrayImageData{std::make_shared<cv::Mat>(res)};
}

QtNodes::ConvertersRegister Get_GrayImageData_conversions() {
    QtNodes::ConvertersRegister converters_register;
    converters_register.add_converter(GrayImageData_from_DecimalData);
    converters_register.add_converter(GrayImageData_from_IntegerData);
    converters_register.add_converter(GrayImageData_from_ColImageData);
    return converters_register;
}

}; // namespace nitro::imcore
