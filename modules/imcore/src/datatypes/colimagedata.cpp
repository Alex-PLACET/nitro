#include "nitro/datatypes/colimagedata.hpp"

#include "nitro/datatypes/grayimagedata.hpp"

#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>

namespace nitro {

ColImageData::ColImageData()
    : FlexibleData<std::shared_ptr<cv::Mat>, ColImageData>(std::make_shared<cv::Mat>(),
                                                           id_,
                                                           name_,
                                                           baseColor_) {
    allowConversionFrom(GrayImageData::id());
}

ColImageData::ColImageData(std::shared_ptr<cv::Mat> img)
    : FlexibleData<std::shared_ptr<cv::Mat>, ColImageData>(std::move(img), id_, name_, baseColor_) {
    allowConversionFrom(GrayImageData::id());
}

ColImageData::ColImageData(const cv::Mat &img)
    : FlexibleData<std::shared_ptr<cv::Mat>, ColImageData>(std::make_shared<cv::Mat>(img),
                                                           id_,
                                                           name_,
                                                           baseColor_) {
    allowConversionFrom(GrayImageData::id());
}

QString ColImageData::getDescription() const {
    return "";
}

bool ColImageData::empty() const {
    return data() == nullptr || data()->empty();
}

void ColImageData::registerConversions() {

    // Every type needs a "conversion" to itself
    ColImageData::registerConversionFrom<ColImageData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                return std::static_pointer_cast<ColImageData>(nodeData)->data();
            });

    ColImageData::registerConversionFrom<DecimalData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                const auto imData = std::static_pointer_cast<DecimalData>(nodeData);
                const double val = imData->data();
                return std::make_shared<cv::Mat>(1, 1, CV_32F, cv::Scalar(val, val, val));
            });

    ColImageData::registerConversionFrom<IntegerData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                const auto imData = std::static_pointer_cast<IntegerData>(nodeData);
                const double val = imData->data() / 255.0;
                return std::make_shared<cv::Mat>(1, 1, CV_32F, cv::Scalar(val, val, val));
            });

    ColImageData::registerConversionFrom<GrayImageData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                const auto imData = std::static_pointer_cast<GrayImageData>(nodeData);
                cv::Mat res;
                cv::cvtColor(*imData->data(), res, cv::COLOR_GRAY2RGB);
                return std::make_shared<cv::Mat>(res);
            });
}

} // namespace nitro
