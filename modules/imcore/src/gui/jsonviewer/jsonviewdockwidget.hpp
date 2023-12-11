#pragma once

#include "jsonviewer.hpp"
#include "nitro/gui/nodeeditor/nitrodockwidget.hpp"

#include <QDockWidget>
#include <QLabel>
#include <QStatusBar>


namespace nitro {
class MainWindow;
}

namespace nitro::ImCore {

class JsonViewer;

class JsonViewDockWidget : public NitroDockWidget {
public:
    explicit JsonViewDockWidget(JsonViewer *jsonViewer, MainWindow *window);

    ~JsonViewDockWidget() override;

private:
    JsonViewer *jsonViewer_;
    void initTitleBar(const MainWindow *window);
};

} // namespace nitro::ImCore
