#include "colormap.hpp"

#include "nitro/datatypes/colimagedata.hpp"
#include "nitro/datatypes/grayimagedata.hpp"
#include "nitro/util/util.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>


namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OPTION_DROPDOWN = "Option";
static inline const QString DISPLAY_LABEL = "Color Label";
static inline const QString OUTPUT_IMAGE = "Image";

static cv::Mat createGradientImage(int width, int height) {
    cv::Mat gradientImage(height, width, CV_8UC1);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            gradientImage.at<uchar>(y, x) = static_cast<uchar>(255 * x / width);
        }
    }

    return gradientImage;
}

ColorMapOperator::ColorMapOperator(QLabel *displayLabel) : displayLabel_(displayLabel) {}

void ColorMapOperator::execute(NodePorts &nodePorts) {
    const int option = nodePorts.getOption(OPTION_DROPDOWN);
    const auto colormapType = static_cast<cv::ColormapTypes>(option);
    const cv::Mat mapLabel = createGradientImage(200, 20);
    cv::applyColorMap(mapLabel, mapLabel, colormapType);
    displayLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(mapLabel, displayImage_)));

    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto img = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    cv::Mat result;

    cv::Mat imIn;
    img->convertTo(imIn, CV_8U, 255);

    cv::applyColorMap(imIn, result, colormapType);
    result.convertTo(result, CV_32F, 1 / 255.0);

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> ColorMapOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Apply Color Map", "colorMap", category);
        auto *displayLabel = new QLabel();
        return builder.withOperator(std::make_unique<ColorMapOperator>(displayLabel))
                ->withIcon("colormap.png")
                ->withNodeColor(NITRO_COLOR_COLOR)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withDropDown(OPTION_DROPDOWN,
                               {"Autumn", "Bone",      "Jet",     "Winter",   "Rainbow",
                                "Ocean",  "Summer",    "Spring",  "Cool",     "Hsv",
                                "Pink",   "Hot",       "Parula",  "Magma",    "Inferno",
                                "Plasma", "Viridis",   "Cividis", "Twilight", "Twilight Shifted",
                                "Turbo",  "Deep green"})
                ->withDisplayWidget(DISPLAY_LABEL, displayLabel)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
