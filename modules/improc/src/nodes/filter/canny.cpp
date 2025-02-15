#include "canny.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_THRESH_1 = "Threshold 1";
static inline const QString INPUT_THRESH_2 = "Threshold 2";
static inline const QString INPUT_APERTURE = "Aperture";
static inline const QString OUTPUT_IMAGE = "Image";

void CannyEdgeDetectionOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // Get the input data
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const double thresh1 = nodePorts.inputValue(INPUT_THRESH_1);
    const double thresh2 = nodePorts.inputValue(INPUT_THRESH_2);
    int aperture = nodePorts.inputInteger(INPUT_APERTURE);
    if (aperture % 2 == 0) {
        aperture -= 1;
    }

    cv::Mat intImage;
    inputImg->convertTo(intImage, CV_8U, 255);

    // Perform filtering
    cv::Mat edges;
    cv::Canny(intImage, edges, thresh1, thresh2, aperture);

    cv::Mat result;
    edges.convertTo(result, CV_32F);

    // Store the result
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters CannyEdgeDetectionOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Canny Edge Detector", "cannyEdgeDetect", category);
        return builder.withOperator(std::make_unique<CannyEdgeDetectionOperator>())
                ->withIcon("blur.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputValue(INPUT_THRESH_1, 75, 1, 255)
                ->withInputValue(INPUT_THRESH_2, 75, 1, 255)
                ->withInputInteger(INPUT_APERTURE, 3, 3, 7)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
