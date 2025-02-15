#include "morphology.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE_1 = "Image";
static inline const QString INPUT_IMAGE_2 = "Kernel";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";

void MorphologyOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_1);
    const auto im2 = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE_2);

    cv::Mat kernel;
    im2->convertTo(kernel, CV_8UC1, 255);
    const int option = nodePorts.getOption(MODE_DROPDOWN);

    cv::Mat result;
    switch (option) {
        case 0:
            cv::dilate(*im1, result, kernel);
            break;
        case 1:
            cv::erode(*im1, result, kernel);
            break;
        case 2:
            cv::morphologyEx(*im1, result, cv::MORPH_OPEN, kernel);
            break;
        case 3:
            cv::morphologyEx(*im1, result, cv::MORPH_CLOSE, kernel);
            break;
        case 4:
            cv::morphologyEx(*im1, result, cv::MORPH_TOPHAT, kernel);
            break;
        case 5:
            cv::morphologyEx(*im1, result, cv::MORPH_BLACKHAT, kernel);
            break;
        default:
            cv::dilate(*im1, result, kernel);
            break;
    }

    result.convertTo(result, CV_32F);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters MorphologyOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Morphology", "morphology", category);
        return builder.withOperator(std::make_unique<MorphologyOperator>())
                ->withIcon("morphology.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withDropDown(MODE_DROPDOWN,
                               {"Dilate", "Erode", "Open", "Close", "Top Hat", "Black Hat"})
                ->withInputPort<ColImageData>(INPUT_IMAGE_1)
                ->withInputPort<GrayImageData>(INPUT_IMAGE_2)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
