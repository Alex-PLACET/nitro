#include "boxfilter.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_SIZE = "Size";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString BORDER_DROPDOWN = "Border";

void BoxFilterOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    int kSize = nodePorts.inputInteger(INPUT_SIZE);
    const int option = nodePorts.getOption(MODE_DROPDOWN);
    const int borderOption = nodePorts.getOption(BORDER_DROPDOWN);

    cv::Mat charImg;
    inputImg->convertTo(charImg, CV_8U, 255);

    cv::Mat resultChar;
    switch (option) {
        case 0:
            cv::blur(charImg, resultChar, cv::Size(kSize, kSize), cv::Point(-1, -1), borderOption);
            break;
        case 1:
            kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
            cv::medianBlur(charImg, resultChar, kSize);
            break;
        default:
            cv::blur(charImg, resultChar, cv::Size(kSize, kSize), cv::Point(-1, -1), borderOption);
            break;
    }

    cv::Mat result;
    resultChar.convertTo(result, CV_32F, 1.0 / 255.0);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters BoxFilterOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Box Filter", "boxFilter", category);
        return builder.withOperator(std::make_unique<BoxFilterOperator>())
                ->withIcon("blur.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withDropDown(MODE_DROPDOWN, {"Average", "Median"})
                ->withDropDown(BORDER_DROPDOWN, {"Constant", "Replicate", "Reflect"})
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_SIZE, 5, 1, 256)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
