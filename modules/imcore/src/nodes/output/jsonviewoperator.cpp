#include "jsonviewoperator.hpp"

#include "gui/jsonviewer/jsonviewdockwidget.hpp"
#include "nitro/core/nodes/datatypes/jsondata.hpp"

#include <QGraphicsScene>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <qwidget.h>

namespace nitro::ImCore {

static inline const QString INPUT_JSON = "Json";

JsonViewOperator::JsonViewOperator(MainWindow *window) : window_(window) {
    if (window->isFinalized()) {
        // TODO:
        jsonViewer_ = new JsonViewer(new QWidget());
        dockWidget_ = new JsonViewDockWidget(jsonViewer_, window);
        window->registerDock(dockWidget_);
    }
}

JsonViewOperator::~JsonViewOperator() {
    if (window_->isFinalized()) {
        window_->removeDockWidget(dockWidget_);
        delete jsonViewer_;
        delete dockWidget_;
    }
}

void JsonViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        if (jsonViewer_ != nullptr) {
            jsonViewer_->clear();
        }
        return;
    }
    const auto json = nodePorts.inGetAs<JsonData>(INPUT_JSON);
    jsonViewer_->updateJson(json);
    nodePorts.setOutputData(INPUT_JSON, nodePorts.inGet(INPUT_JSON));
}

CreatorWithoutParameters JsonViewOperator::creator(const QString &category, MainWindow *window) {
    return [category,
            window](const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Json Viewer", "JsonViewer", category);
        return builder.withOperator(std::make_unique<JsonViewOperator>(window))
                ->withIcon("viewer.png")
                ->withNodeColor(NITRO_OUTPUT_COLOR)
                ->withInputPort<JsonData>(INPUT_JSON)
                ->withOutputPort<JsonData>(INPUT_JSON)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
