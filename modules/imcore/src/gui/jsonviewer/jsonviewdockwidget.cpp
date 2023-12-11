#include "jsonviewdockwidget.hpp"

#include "jsonviewer.hpp"

#include <nitro/gui/nodeeditor/mainwindow.hpp>

namespace nitro::ImCore {

JsonViewDockWidget::JsonViewDockWidget(JsonViewer *jsonViewer, MainWindow *window)
    : NitroDockWidget("JSON Viewer", window),
      jsonViewer_(jsonViewer) {
    setWidget(jsonViewer_);
    initTitleBar(window);
}

void JsonViewDockWidget::initTitleBar(const MainWindow *window) {
    setIcon(":/icons/image_viewer.png");
}

JsonViewDockWidget::~JsonViewDockWidget() = default;

} // namespace nitro::ImCore
