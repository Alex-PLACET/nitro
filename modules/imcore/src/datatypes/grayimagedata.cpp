#include "nitro/datatypes/grayimagedata.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>


namespace nitro {

GrayImageData::GrayImageData()
    : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::make_shared<cv::Mat>(),
                                                            id_,
                                                            name_,
                                                            baseColor_) {
}

GrayImageData::GrayImageData(std::shared_ptr<cv::Mat> img)
    : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::move(img),
                                                            id_,
                                                            name_,
                                                            baseColor_) {
}

GrayImageData::GrayImageData(const cv::Mat &img)
    : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::make_shared<cv::Mat>(img),
                                                            id_,
                                                            name_,
                                                            baseColor_) {
}

QString GrayImageData::getDescription() const {
    return "";
}

bool GrayImageData::empty() const {
    return data() == nullptr || data()->empty();
}

} // namespace nitro
