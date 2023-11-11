#include "imtranslate.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

const inline QString INPUT_IMAGE = "Image";
const inline QString INPUT_X = "X";
const inline QString INPUT_Y = "Y";
const inline QString OPTION_PAD = "Pad";
const inline QString OUTPUT_IMAGE = "Image";
const inline QString INTERPOL_METHOD_LABEL = "Interpolation Method";

static cv::Mat translateImagePad(const cv::Mat &image, int dx, int dy) {

    const int newWidth = image.cols + std::abs(dx);
    const int newHeight = image.rows + std::abs(dy);

    dx = std::max(dx, 0);
    dy = std::max(dy, 0);

    const cv::Mat translationMatrix = (cv::Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
    cv::Mat translatedImage;
    cv::warpAffine(image, translatedImage, translationMatrix, {newWidth, newHeight});
    return translatedImage;
}

static cv::Mat translateImage(const cv::Mat &image, int dx, int dy) {
    const cv::Mat translationMatrix = (cv::Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
    cv::Mat translatedImage;
    cv::warpAffine(image, translatedImage, translationMatrix, image.size());
    return translatedImage;
}

void TranslateOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    const double offsetRatioX = nodePorts.inputValue(INPUT_X);
    const double offsetRatioY = nodePorts.inputValue(INPUT_Y);
    const int offsetX = int(std::round(offsetRatioX * im1->cols));
    const int offsetY = int(std::round(offsetRatioY * im1->rows));
    const int option = nodePorts.getOption(OPTION_PAD);

    cv::Mat result;
    if (option) {
        result = translateImagePad(*im1, offsetX, offsetY);
    } else {
        result = translateImage(*im1, offsetX, offsetY);
    }
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> TranslateOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Translate", "translate", category);
        return builder.withOperator(std::make_unique<TranslateOperator>())
                ->withIcon("resize.png")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withCheckBox(OPTION_PAD, false)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputValue(INPUT_X, 0, -1, 1, BoundMode::UNCHECKED)
                ->withInputValue(INPUT_Y, 0, -1, 1, BoundMode::UNCHECKED)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
