#include "distancetransform.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/datatypes/grayimagedata.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_THRESH = "Threshold";
static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString OPTION_SIGNED = "Signed";
static inline const QString OPTION_NORMALIZE = "Normalize";

static cv::Mat distanceField(cv::Mat const &src,
                             double threshold,
                             cv::DistanceTypes mode,
                             bool signedDf,
                             bool normalize) {
    cv::Mat binaryImOutside;
    cv::threshold(src, binaryImOutside, threshold, 1, cv::THRESH_BINARY_INV);
    binaryImOutside.convertTo(binaryImOutside, CV_8UC1, 255);

    cv::Mat df;
    cv::distanceTransform(binaryImOutside, df, mode, cv::DIST_MASK_PRECISE);
    if (normalize) {
        cv::normalize(df, df, 0, 1, cv::NORM_MINMAX);
    }
    if (signedDf) {
        cv::Mat binaryImInside;
        cv::threshold(src, binaryImInside, threshold, 1, cv::THRESH_BINARY);
        binaryImInside.convertTo(binaryImInside, CV_8UC1, 255);
        cv::Mat dfIn;
        cv::distanceTransform(binaryImInside, dfIn, mode, cv::DIST_MASK_PRECISE);
        if (normalize) {
            cv::normalize(dfIn, dfIn, 0, 1, cv::NORM_MINMAX);
        }
        cv::subtract(df, dfIn, df);
    }
    df.convertTo(df, CV_32F);
    return df;
}

void DistanceTransformOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    const auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    const double threshold = nodePorts.inputValue(INPUT_THRESH);
    const int mode = nodePorts.getOption(MODE_DROPDOWN);
    const int signedDf = nodePorts.getOption(OPTION_SIGNED);
    const bool normalize = nodePorts.optionEnabled(OPTION_NORMALIZE);

    cv::DistanceTypes type;
    switch (mode) {
        case 0:
            type = cv::DIST_L2;
            break;
        case 1:
            type = cv::DIST_L1;
            break;
        case 2:
            type = cv::DIST_C;
            break;
        case 3:
            type = cv::DIST_FAIR;
            break;
        case 4:
            type = cv::DIST_WELSCH;
            break;
        case 5:
            type = cv::DIST_HUBER;
            break;
        default:
            type = cv::DIST_L1;
    }

    cv::Mat result = distanceField(*inputImg, threshold, type, signedDf, normalize);
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

CreatorWithoutParameters DistanceTransformOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Distance Transform", "distanceTransform", category);
        return builder.withOperator(std::make_unique<DistanceTransformOperator>())
                ->withIcon("distance.png")
                ->withNodeColor(NITRO_ANALYSIS_COLOR)
                ->withInputValue(INPUT_THRESH, 0.5, 0, 1)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withDropDown(MODE_DROPDOWN, {"Euclidean", "Manhattan", "Chessboard"})
                ->withCheckBox(OPTION_SIGNED, false)
                ->withCheckBox(OPTION_NORMALIZE, false)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImProc
