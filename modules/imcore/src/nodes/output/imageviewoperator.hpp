#pragma once

#include "gui/imviewer/imageviewer.hpp"
#include "nitro/gui/nodeeditor/mainwindow.hpp"

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class ImageViewOperator : public NodeOperator {
public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category,
                                                               MainWindow *window);

    explicit ImageViewOperator(MainWindow *window);

    ~ImageViewOperator();

    void execute(NodePorts &nodePorts) override;

private:
    ImageViewer *imViewer_ = nullptr;
    QDockWidget *dockWidget_;
    MainWindow *window_;
};

} // namespace nitro::ImCore
