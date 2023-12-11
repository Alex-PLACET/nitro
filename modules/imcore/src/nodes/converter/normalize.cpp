#include "normalize.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_MIN = "Min";
static inline const QString INPUT_MAX = "Max";
static inline const QString OUTPUT_IMAGE = "Image";

void NormalizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const double min = nodePorts.inputValue(INPUT_MIN);
    const double max = nodePorts.inputValue(INPUT_MAX);
    cv::Mat result;
    cv::normalize(*inputImg, result, min, max, cv::NORM_MINMAX);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters NormalizeOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Normalize", "normalize", category);
        return builder.withOperator(std::make_unique<NormalizeOperator>())
                ->withIcon("map_range.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputValue(INPUT_MIN, 0, 0, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_MAX, 1, 0, 1, BoundMode::UNCHECKED)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
