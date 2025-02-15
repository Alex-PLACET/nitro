#include "fouriertransform.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString OPTION_INVERSE = "Inverse";

void FFTOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    const auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    const int inverse = nodePorts.getOption(OPTION_INVERSE);

    cv::Mat result;
    cv::dft(*inputImg, result, inverse);

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters FFTOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Fourier Transform", "fft", category);
        return builder.withOperator(std::make_unique<FFTOperator>())
                ->withIcon("frequency.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withCheckBox(OPTION_INVERSE, false)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
