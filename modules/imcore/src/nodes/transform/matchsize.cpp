#include "matchsize.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_IMAGE_TARGET = "Target";
static inline const QString ASPECT_RATIO_DROPDOWN = "Keep Aspect Ratio";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString INTERPOL_METHOD_LABEL = "Interpolation Method";
static inline const QString AR_METHOD_LABEL = "Aspect Ratio Method";

void MatchSizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const AspectRatioMode arMode = static_cast<AspectRatioMode>(
            nodePorts.getOption(ASPECT_RATIO_DROPDOWN));
    const auto imIn = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const auto imTarget = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_TARGET);
    const int option = nodePorts.getOption(MODE_DROPDOWN);
    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    const cv::Mat result = nitro::resize(*imIn, imTarget->size(), mode, arMode);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters MatchSizeOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Match Size", "matchSize", category);
        return builder.withOperator(std::make_unique<MatchSizeOperator>())
                ->withIcon("match_size.png")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withDisplayWidget(INTERPOL_METHOD_LABEL, "Interpolation Method:")
                ->withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})
                ->withDisplayWidget(AR_METHOD_LABEL, "Aspect Ratio:")
                ->withDropDown(ASPECT_RATIO_DROPDOWN,
                               {"Ignore", "Keep Crop", "Keep Shrink", "Keep Grow"})
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputPort<ColImageData>(INPUT_IMAGE_TARGET)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
