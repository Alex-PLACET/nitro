#pragma once

#include "nitro/gui/nodeeditor/mainwindow.hpp"
#include "src/gui/histogram/histogramviewer.hpp"

#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class HistogramViewOperator : public NodeOperator {
public:
    static CreatorWithoutParameters creator(const QString &category, MainWindow *window);

    explicit HistogramViewOperator(MainWindow *window);
    ~HistogramViewOperator() override;

    void execute(NodePorts &nodePorts) override;

private:
    HistogramView *histViewer_ = nullptr;
    MainWindow *window_;
    QDockWidget *dockWidget_;
};

} // namespace nitro::ImCore
