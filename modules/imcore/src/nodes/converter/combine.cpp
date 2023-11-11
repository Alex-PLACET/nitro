#include "combine.hpp"

#include "nitro/datatypes/colimagedata.hpp"
#include "nitro/datatypes/grayimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString OUTPUT_IMAGE = "Image";

void CombineOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    std::vector<cv::Mat> channels;
    channels.resize(3);
    for (size_t i = 0; i < channels.size(); i++) {
        const QString name = QString("Channel %1").arg(i + 1);
        const auto inputImg = nodePorts.inGetAs<GrayImageData>(name);
        channels[i] = *inputImg;
    }
    for (size_t i = 1; i < channels.size(); i++) {
        if (channels[i].size != channels[0].size || channels[i].depth() != channels[0].depth()) {
            return;
        }
    }
    cv::Mat result;
    cv::merge(channels, result);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> CombineOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Combine Channels", "combineChannels", category);
        return builder.withOperator(std::make_unique<CombineOperator>())
                ->withIcon("layers.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<GrayImageData>("Channel 1")
                ->withInputPort<GrayImageData>("Channel 2")
                ->withInputPort<GrayImageData>("Channel 3")
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
