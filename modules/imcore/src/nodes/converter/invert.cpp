#include "invert.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>


namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";

void InvertOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    cv::Mat result = cv::abs(1 - *inputImg);
    if (inputImg->channels() == 1) {
        result = cv::abs(1 - *inputImg);
    } else {
        std::vector<cv::Mat> channels;
        cv::split(*inputImg, channels);
        std::vector<cv::Mat> outChannels;
        outChannels.resize(channels.size());
        for (size_t i = 0; i < channels.size(); i++) {
            outChannels[i] = cv::abs(1 - channels[i]);
        }
        cv::merge(outChannels, result);
    }
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> InvertOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Invert", "invert", category);
        return builder.withOperator(std::make_unique<InvertOperator>())
                ->withIcon("invert.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
