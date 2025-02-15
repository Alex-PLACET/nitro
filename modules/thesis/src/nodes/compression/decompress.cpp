#include "decompress.hpp"

#include "nodes/restoration/resample.hpp"

#include <nitro/datatypes/grayimagedata.hpp>
#include <nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>
#include <util.hpp>


namespace nitro::Thesis {

static inline const QString INPUT_IMAGE = "Residual";
static inline const QString INPUT_SMALL = "Small";
static inline const QString INPUT_K_SIZE = "Filter Size";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString UNIFORM_LUM = "Uniform Luminance";
static inline const QString TIME_LABEL = "Time";

static cv::Mat toRgb(const cv::Mat &img) {
    cv::Mat result;
    cv::cvtColor(img, result, cv::COLOR_GRAY2RGB);
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    channels[0] = channels[0] * 100.0;
    cv::merge(channels, result);
    cvtColor(result, result, cv::COLOR_Lab2RGB);
    cv::split(result, channels);
    return channels[0];
}

void DecompressOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // Get the input data
    auto residual = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    auto smallImg = *nodePorts.inGetAs<GrayImageData>(INPUT_SMALL);
    const double filterSize = nodePorts.inputValue(INPUT_K_SIZE);

    const double start = cv::getTickCount();

    // Small image
    cv::Mat largeMain;
    cv::GaussianBlur(smallImg, largeMain, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_REFLECT);
    cv::resize(largeMain, largeMain, residual.size(), 0, 0, cv::INTER_LINEAR);

    // Residual
    cv::Mat result = (resampleImage(residual, true, filterSize) * 2.0) - 1.0 + largeMain;

    if (nodePorts.optionEnabled(UNIFORM_LUM)) {
        result = toRgb(result);
    }

     const double end = cv::getTickCount();
     const double elapsedTime = (end - start) / cv::getTickFrequency() * 1000.0;
     timeLabel_->setText(QString("Time: %1 msec").arg(elapsedTime));
     // Store the result
     nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters DecompressOperator::creator(const QString &category) {
    return [category]() {
        auto *timeLabel = new QLabel("-");
        NitroNodeBuilder builder("Bit Decompress", "bitDecompress", category);
        return builder.withOperator(std::make_unique<DecompressOperator>(timeLabel))
                ->withIcon("expand.png")
                ->withNodeColor(NITRO_COMPRESSION_COLOR)
                ->withDisplayWidget(TIME_LABEL, timeLabel)
                ->withCheckBox(UNIFORM_LUM, false)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withInputPort<GrayImageData>(INPUT_SMALL)
                ->withInputValue(INPUT_K_SIZE, 1.0, 0, 1, BoundMode::UPPER_LOWER)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

DecompressOperator::DecompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}

} // namespace nitro::Thesis
