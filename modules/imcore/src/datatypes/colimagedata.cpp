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
}

ColImageData::ColImageData(std::shared_ptr<cv::Mat> img)
    : FlexibleData<std::shared_ptr<cv::Mat>, ColImageData>(std::move(img), id_, name_, baseColor_) {
}

ColImageData::ColImageData(const cv::Mat &img)
    : FlexibleData<std::shared_ptr<cv::Mat>, ColImageData>(std::make_shared<cv::Mat>(img),
                                                           id_,
                                                           name_,
                                                           baseColor_) {
}

QString ColImageData::getDescription() const {
    return ""; // TODO: Image size
}

bool ColImageData::empty() const {
    return data() == nullptr || data()->empty();
}

} // namespace nitro
