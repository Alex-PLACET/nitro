#include "maprange.hpp"

#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Value";
static inline const QString INPUT_FROM_MIN = "From Min";
static inline const QString INPUT_FROM_MAX = "From Max";
static inline const QString INPUT_TO_MIN = "To Min";
static inline const QString INPUT_TO_MAX = "To Max";
static inline const QString OUTPUT_IMAGE = "Result";
static inline const QString OPTION_CLAMP = "Clamp";

void MapRangeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const double fromMin = nodePorts.inputValue(INPUT_FROM_MIN);
    const double fromMax = nodePorts.inputValue(INPUT_FROM_MAX);
    const double toMin = nodePorts.inputValue(INPUT_TO_MIN);
    const double toMax = nodePorts.inputValue(INPUT_TO_MAX);
    if (nodePorts.inputOfType<DecimalData>(INPUT_IMAGE)) {
        double value = nodePorts.inputValue(INPUT_IMAGE);
        value = (value - fromMin) / (fromMax - fromMin);
        value = std::lerp(toMin, toMax, value);
        if (nodePorts.optionEnabled(OPTION_CLAMP)) {
            value = std::clamp(value, toMin, toMax);
        }
        nodePorts.output<DecimalData>(OUTPUT_IMAGE, value);
        return;
    }
    const auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    cv::Mat result = (*inputImg - fromMin) / (fromMax - fromMin);
    result = toMin + result * (toMax - toMin);

    if (nodePorts.optionEnabled(OPTION_CLAMP)) {
        cv::Scalar upper;
        cv::Scalar lower;
        if (result.channels() == 3) {
            upper = {toMax, toMax, toMax};
            lower = {toMin, toMin, toMin};
        } else {
            upper = {toMax};
            lower = {toMin};
        }
        result = cv::min(result, upper);
        result = cv::max(result, lower);
    }

    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters MapRangeOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Map Range", "mapRange", category);
        return builder.withOperator(std::make_unique<MapRangeOperator>())
                ->withIcon("map_range.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputValue(INPUT_IMAGE, 0.5, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_FROM_MIN, 0, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_FROM_MAX, 1, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_TO_MIN, 0, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_TO_MAX, 1, 0, 1, BoundMode::UNCHECKED)
                ->withOutputValue(OUTPUT_IMAGE)
                ->withCheckBox(OPTION_CLAMP, false)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
