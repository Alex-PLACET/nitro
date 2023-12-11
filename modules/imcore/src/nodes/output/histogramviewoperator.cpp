#include "histogramviewoperator.hpp"

#include "gui/histogram/histogramdockwidget.hpp"
#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";

HistogramViewOperator::HistogramViewOperator(MainWindow *window) : window_(window) {
    if (window->isFinalized()) {
        histViewer_ = new HistogramView();
        dockWidget_ = new HistogramViewerDockWidget(histViewer_, window);
        window->registerDock(dockWidget_);
    }
}

HistogramViewOperator::~HistogramViewOperator() {
    if (window_->isFinalized()) {
        window_->removeDockWidget(dockWidget_);
        delete histViewer_;
        delete dockWidget_;
    }
}

void HistogramViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        if (histViewer_ != nullptr) {
            histViewer_->updateChart(cv::Mat());
        }
        return;
    }
    auto im = *nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    if (im.rows == 1 && im.cols == 1) {
        cv::resize(im, im, {256, 256});
    }
    histViewer_->updateChart(im);
}

CreatorWithoutParameters HistogramViewOperator::creator(const QString &category,
                                                        MainWindow *window) {
    return [category,
            window](const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Histogram", "HistogramViewer", category);
        return builder.withOperator(std::make_unique<HistogramViewOperator>(window))
                ->withIcon("hist.png")
                ->withNodeColor(NITRO_OUTPUT_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
