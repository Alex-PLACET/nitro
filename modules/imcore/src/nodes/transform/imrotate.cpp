#include "imrotate.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";

void ImRotateOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const int option = nodePorts.getOption(MODE_DROPDOWN);
    const auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

    cv::RotateFlags mode;
    switch (option) {
        case 0:
            mode = cv::ROTATE_90_CLOCKWISE;
            break;
        case 1:
            mode = cv::ROTATE_90_COUNTERCLOCKWISE;
            break;
        default:
            mode = cv::ROTATE_180;
            break;
    }

    cv::Mat result;
    cv::rotate(*im1, result, mode);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters ImRotateOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Rotate", "rotate", category);
        return builder.withOperator(std::make_unique<ImRotateOperator>())
                ->withIcon("rotate.png")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withDropDown(MODE_DROPDOWN, {"90 (CW)", "90 (CCw)", "180"})
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
