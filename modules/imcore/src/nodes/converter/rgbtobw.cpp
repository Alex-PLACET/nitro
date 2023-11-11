#include "rgbtobw.hpp"

#include "nitro/datatypes/colimagedata.hpp"
#include "nitro/datatypes/grayimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";

void RgbToGrayscaleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    cv::Mat result;
    if (inputImg->channels() == 1) {
        inputImg->copyTo(result);
    } else {
        cvtColor(*inputImg, result, cv::COLOR_RGB2GRAY);
    }
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> RgbToGrayscaleOperator::creator(
        const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("RGB to Grayscale", "grayscaleConvert", category);
        return builder.withOperator(std::make_unique<RgbToGrayscaleOperator>())
                ->withIcon("greyscale.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
