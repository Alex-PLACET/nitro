#include "mask.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>


namespace nitro::ImProc {

static inline const QString INPUT_MASK_WIDTH = "Scale X";
static inline const QString INPUT_MASK_HEIGHT = "Scale Y";
static inline const QString INPUT_X = "Width";
static inline const QString INPUT_Y = "Height";
static inline const QString INPUT_POS_X = "X";
static inline const QString INPUT_POS_Y = "Y";
static inline const QString OUTPUT_IMAGE = "Mask";
static inline const QString MODE_DROPDOWN = "Mode";

// TODO: rotation
void MaskOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const int option = nodePorts.getOption(MODE_DROPDOWN);
    const int width = nodePorts.inputInteger(INPUT_X);
    const int height = nodePorts.inputInteger(INPUT_Y);
    const double innerWidth = nodePorts.inputValue(INPUT_MASK_WIDTH);
    const double innerHeight = nodePorts.inputValue(INPUT_MASK_HEIGHT);
    const double posX = nodePorts.inputValue(INPUT_POS_X);
    const double posY = nodePorts.inputValue(INPUT_POS_Y);
    const int kernelWidth = int(std::round(innerWidth * width / 2.0));
    const int kernelHeight = int(std::round(innerHeight * height / 2.0));
    const cv::Mat image(height, width, CV_8UC1, cv::Scalar(0));
    if (kernelHeight > 0 && kernelWidth > 0) {
        const int centerX = int(std::round(width * posX));
        const int centerY = int(std::round(height * posY));
        if (option == 0) {
            cv::ellipse(image,
                        {centerX, centerY},
                        {kernelWidth, kernelHeight},
                        0,
                        0,
                        360,
                        cv::Scalar(255),
                        -1);
        } else {
            const cv::Point v1 = {centerX - kernelWidth, centerY - kernelHeight};
            const cv::Point v2 = {centerX + kernelWidth, centerY + kernelHeight};
            cv::rectangle(image, v1, v2, cv::Scalar(255), -1);
        }
    }

    cv::Mat result;
    image.convertTo(result, CV_32FC1, 1 / 255.0);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters MaskOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Mask", "mask", category);
        return builder.withOperator(std::make_unique<MaskOperator>())
                ->withIcon("mask.png")
                ->withNodeColor(NITRO_INPUT_COLOR)
                ->withDropDown(MODE_DROPDOWN, {"Ellipse", "Rectangle"})
                ->withInputInteger(INPUT_X, 256, 1, 4096)
                ->withInputInteger(INPUT_Y, 256, 1, 4096)
                ->withInputValue(INPUT_MASK_WIDTH, 1, 0, 2, BoundMode::LOWER_ONLY)
                ->withInputValue(INPUT_MASK_HEIGHT, 1, 0, 2, BoundMode::LOWER_ONLY)
                ->withInputValue(INPUT_POS_X, 0.5, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_POS_Y, 0.5, 0, 1, BoundMode::UNCHECKED)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
