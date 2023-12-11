#pragma once

#include "gui/imviewer/imageviewer.hpp"

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>


namespace nitro::ImCore {

class ImageViewOperator : public NodeOperator {
public:
    static CreatorWithoutParameters creator(const QString &category, MainWindow *window);

    explicit ImageViewOperator(MainWindow *window);

    ~ImageViewOperator();

    void execute(NodePorts &nodePorts) override;

private:
    ImageViewer *imViewer_ = nullptr;
    QDockWidget *dockWidget_;
    MainWindow *window_;
};

} // namespace nitro::ImCore
