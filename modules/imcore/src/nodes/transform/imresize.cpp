#include "imresize.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_X = "Width";
static inline const QString INPUT_Y = "Height";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString ASPECT_RATIO_DROPDOWN = "Keep Aspect Ratio";
static inline const QString INTERPOL_METHOD_LABEL = "Interpolation Method";
static inline const QString AR_METHOD_LABEL = "Aspect Ratio Method";

void ResizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const AspectRatioMode arMode = static_cast<AspectRatioMode>(
            nodePorts.getOption(ASPECT_RATIO_DROPDOWN));
    const int option = nodePorts.getOption(MODE_DROPDOWN);
    const auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const int width = nodePorts.inputInteger(INPUT_X);
    const int height = nodePorts.inputInteger(INPUT_Y);
    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    const cv::Mat result = resize(*im1, cv::Size(width, height), mode, arMode);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters ResizeOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Resize", "resize", category);
        return builder.withOperator(std::make_unique<ResizeOperator>())
                ->withIcon("resize.png")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withDisplayWidget(INTERPOL_METHOD_LABEL, "Interpolation Method:")
                ->withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})
                ->withDisplayWidget(AR_METHOD_LABEL, "Aspect Ratio:")
                ->withDropDown(ASPECT_RATIO_DROPDOWN,
                               {"Ignore", "Keep Crop", "Keep Shrink", "Keep Grow"})
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_X, 256, 2, 2048, BoundMode::LOWER_ONLY)
                ->withInputInteger(INPUT_Y, 256, 2, 2048, BoundMode::LOWER_ONLY)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
