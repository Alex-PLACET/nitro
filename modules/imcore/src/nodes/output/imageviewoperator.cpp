#include "imageviewoperator.hpp"

#include "gui/imviewer/imviewdockwidget.hpp"
#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";

ImageViewOperator::ImageViewOperator(MainWindow *window) : window_(window) {
    if (window->isFinalized()) {
        // TODO:
        imViewer_ = new ImageViewer(new QGraphicsScene());
        dockWidget_ = new ImViewDockWidget(imViewer_, window);
        window->registerDock(dockWidget_);
    }
}

ImageViewOperator::~ImageViewOperator() {
    if (window_->isFinalized()) {
        window_->removeDockWidget(dockWidget_);
        delete imViewer_;
        delete dockWidget_;
    }
}

void ImageViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        if (imViewer_ != nullptr) {
            imViewer_->removeImage();
        }
        return;
    }
    const auto im = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

    if (im->rows == 1 && im->cols == 1) {
        cv::resize(*im, *im, {256, 256});
    }
    imViewer_->setImage(im);
    nodePorts.setOutputData(INPUT_IMAGE, nodePorts.inGet(INPUT_IMAGE));
}

CreatorWithoutParameters ImageViewOperator::creator(const QString &category, MainWindow *window) {
    return [category,
            window](const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Image Viewer", "ImageViewer", category);
        return builder.withOperator(std::make_unique<ImageViewOperator>(window))
                ->withIcon("viewer.png")
                ->withNodeColor(NITRO_OUTPUT_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withOutputPort<ColImageData>(INPUT_IMAGE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
