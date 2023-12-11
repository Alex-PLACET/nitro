#include "bilateralfilter.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_SIGMA_C = "Sigma (c)";
static inline const QString INPUT_SIGMA_S = "Sigma (s)";
static inline const QString INPUT_D = "Diameter";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";

void BilateralFilterOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    const auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const double sigmaCol = nodePorts.inputValue(INPUT_SIGMA_C);
    const double sigmaSpace = nodePorts.inputValue(INPUT_SIGMA_S);
    const int d = nodePorts.inputInteger(INPUT_D);

    // Perform filtering
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);

    // Store the result
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters BilateralFilterOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.withOperator(std::make_unique<BilateralFilterOperator>())
                ->withIcon("blur.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_D, 9, 1, 64)
                ->withInputValue(INPUT_SIGMA_C, 75, 2, 255)
                ->withInputValue(INPUT_SIGMA_S, 75, 2, 255)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
