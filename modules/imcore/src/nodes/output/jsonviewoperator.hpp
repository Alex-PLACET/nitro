#pragma once

#include "gui/jsonviewer/jsonviewer.hpp"

#include <nitro/core/nodes/nitronode.hpp>
#include <nitro/core/nodes/node_creator.hpp>
#include <nitro/core/nodes/nodeoperator.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>

namespace nitro::ImCore {

class JsonViewOperator final : public NodeOperator {
public:
    static CreatorWithoutParameters creator(const QString &category, MainWindow *window);

    explicit JsonViewOperator(MainWindow *window);

    ~JsonViewOperator() final;

    void execute(NodePorts &nodePorts) override;

private:
    JsonViewer *jsonViewer_ = nullptr;
    QDockWidget *dockWidget_;
    MainWindow *window_;
};

} // namespace nitro::ImCore
