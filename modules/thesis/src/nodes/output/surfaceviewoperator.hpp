#pragma once

#include "gui/im3dviewer/renderview.hpp"
#include "gui/mainwindow.hpp"

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class SurfaceViewOperator : public NodeOperator {

public:
    static CreatorWithoutParameters creator(const QString &category, MainWindow *window);

    explicit SurfaceViewOperator(MainWindow *window);

    ~SurfaceViewOperator();

    void execute(NodePorts &nodePorts) override;

private:
    MainWindow *window_;
    QDockWidget *dockWidget_;
    RenderView *surfViewer_;
    cv::Mat currentImg_;
};
} // namespace nitro::Thesis
