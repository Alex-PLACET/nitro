#include "denoise.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";

void DenoiseOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

    cv::Mat grayImage;
    inputImg->convertTo(grayImage, CV_8U, 255);

    cv::Mat result;
    cv::fastNlMeansDenoising(grayImage, result);

    result.convertTo(result, CV_32F, 1.0 / 255.0);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> DenoiseOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Denoise", "denoise", category);
        return builder.withOperator(std::make_unique<DenoiseOperator>())
                ->withIcon("denoise.png")
                ->withNodeColor(NITRO_RESTORATION_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImProc
