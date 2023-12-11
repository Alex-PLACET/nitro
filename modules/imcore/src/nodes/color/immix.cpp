#include "immix.hpp"

#include "nitro/datatypes/colimagedata.hpp"
#include "nitro/datatypes/grayimagedata.hpp"

#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_FAC = "Fac";
static inline const QString INPUT_VALUE_1 = "Value 1";
static inline const QString INPUT_VALUE_2 = "Value 2";
static inline const QString OUTPUT_VALUE = "Value";
static inline const QString OPTION_CLAMP = "Clamp";
static inline const QString MODE_DROPDOWN = "Mode";

// Note this function doesn't support alpha images
static void match(const cv::Mat &src, cv::Mat &dest, const cv::Size &size, int numChannels) {
    src.copyTo(dest);
    if (dest.size() != size) {
        cv::resize(dest, dest, size);
    }
    if (dest.channels() != numChannels) {
        int conversionCode = 0;
        if (numChannels == 1 && dest.channels() == 3) {
            conversionCode = cv::COLOR_RGB2GRAY;
        } else if (numChannels == 3 && dest.channels() == 1) {
            conversionCode = cv::COLOR_GRAY2RGB;
        }
        cv::cvtColor(dest, dest, conversionCode);
    }
}

// ensures the images all have the same size and number of channels
void MixOperator::initUnifiedInputs(const NodePorts &nodePorts) {
    const auto fac = *nodePorts.inGetAs<GrayImageData>(INPUT_FAC);
    const auto in1 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_1);
    const auto in2 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_2);

    const int numChannels = std::max({fac.channels(), in1.channels(), in2.channels()});

    const int area1 = fac.cols * fac.rows;
    const int area2 = in1.cols * in1.rows;
    const int area3 = in2.cols * in2.rows;

    const int maxArea = std::max({area1, area2, area3});
    cv::Size size;
    if (maxArea == area1) {
        size = fac.size();
    } else if (maxArea == area2) {
        size = in1.size();
    } else {
        size = in2.size();
    }

    fac.copyTo(fac_);
    if (fac_.size() != size) {
        cv::resize(fac_, fac_, size);
    }
    match(in1, in1_, size, numChannels);
    match(in2, in2_, size, numChannels);
}

void MixOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const int option = nodePorts.getOption(MODE_DROPDOWN);

    initUnifiedInputs(nodePorts);
    cv::Mat result;
    switch (option) {
        case 0: {
            cv::blendLinear(in1_, in2_, fac_, 1 - fac_, result);
            break;
        }
        default:
            cv::blendLinear(in1_, in2_, fac_, 1 - fac_, result);
            break;
    }

    if (nodePorts.optionEnabled(OPTION_CLAMP)) {
        cv::Scalar upper;
        cv::Scalar lower;
        if (result.channels() == 3) {
            upper = {1, 1, 1};
            lower = {0, 0, 0};
        } else {
            upper = {1};
            lower = {0};
        }
        result = cv::min(result, upper);
        result = cv::max(result, lower);
    }

    nodePorts.output<ColImageData>(OUTPUT_VALUE, result);
}

CreatorWithoutParameters MixOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Mix RGB", "mixRgb", category);
        return builder.withOperator(std::make_unique<MixOperator>())
                ->withIcon("mix.png")
                ->withNodeColor(NITRO_COLOR_COLOR)
                ->withDropDown(MODE_DROPDOWN, {"Mix"})
                ->withInputValue(INPUT_FAC, 1, 0, 1, BoundMode::UPPER_LOWER)
                ->withInputPort<ColImageData>(INPUT_VALUE_1)
                ->withInputPort<ColImageData>(INPUT_VALUE_2)
                ->withOutputValue(OUTPUT_VALUE)
                ->withCheckBox(OPTION_CLAMP, false)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
