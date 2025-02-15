#include "quantize.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/colimagedata.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>


namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_K = "K";
static inline const QString OUTPUT_IMAGE = "Image";

void QuantizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto img = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const int k = nodePorts.inputInteger(INPUT_K) - 1;

    cv::Mat result = *img * k;
    cv::Mat imIn;
    result.convertTo(imIn, CV_8U);
    imIn.convertTo(result, CV_32F, 1.0 / k);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters QuantizeOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Quantize", "quantization", category);
        return builder.withOperator(std::make_unique<QuantizeOperator>())
                ->withIcon("quantize.png")
                ->withNodeColor(NITRO_COMPRESSION_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_K, 8, 2, 255)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
