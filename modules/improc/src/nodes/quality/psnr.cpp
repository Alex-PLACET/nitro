#include "psnr.hpp"

#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_IMAGE_REF = "Reference";
static inline const QString OUTPUT_VALUE = "Error";

static double mse(const cv::Mat &im1, const cv::Mat &im2) {
    cv::Mat diff;
    if (im1.size != im2.size) {
        return -1;
    }
    cv::absdiff(im1, im2, diff);
    const cv::Mat squaredDiff = diff.mul(diff);
    return cv::mean(squaredDiff).val[0];
}

void PsnrOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    const auto imIn = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    const auto imRef = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE_REF);
    const double psnr = 10.0 * log10(1.0 / mse(imIn, imRef));
    nodePorts.output<DecimalData>(OUTPUT_VALUE, psnr);
}

std::function<std::unique_ptr<NitroNode>()> PsnrOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("PSNR", "psnr", category);
        return builder.withOperator(std::make_unique<PsnrOperator>())
                ->withIcon("compare.png")
                ->withNodeColor(NITRO_QUALITY_COLOR)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withInputPort<GrayImageData>(INPUT_IMAGE_REF)
                ->withOutputValue(OUTPUT_VALUE)
                ->build();
    };
}

} // namespace nitro::ImProc
