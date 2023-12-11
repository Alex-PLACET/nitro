#include "nitro/gui/nodeeditor/stylepresets.hpp"

#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QStyle>
#include <QSurfaceFormat>
#include <QtNodes/internal/WidgetNodePainter.hpp>
#include <memory>
#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/noderegistry.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>
#include <nitro/modules/module_list.hpp>
#include <nitro/version.hpp>
#include <qpluginloader.h>

template<class>
inline constexpr bool always_false_v = false;

void setupApplication() {
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
    nitro::applyStylePreset(nitro::getDarkModePalette());

    QFont font("Lato");
    font.setPixelSize(14);
    QApplication::setFont(font);
}

void RegisterNodes(nitro::NodeRegistry &node_registry,
                   const std::vector<nitro::CreatorVariant> &creators,
                   nitro::MainWindow &window) {
    for (const auto &creator: creators) {
        std::visit(
                [&node_registry, &window](auto &&node_arg) {
                    using T = std::decay_t<decltype(node_arg)>;
                    if constexpr (std::is_same_v<T, nitro::CreatorWithWindow>) {
                        node_registry.registerNode(node_arg(&window));
                    } else if constexpr (std::is_same_v<T, nitro::CreatorWithoutParameters>) {
                        node_registry.registerNode(node_arg);
                    } else {
                        static_assert(always_false_v<T>, "non-exhaustive visitor!");
                    }
                },
                creator);
    }
}

void LoadPlugins(nitro::NodeRegistry &node_registry, nitro::MainWindow &window) {
    const QDir pluginsDir(QCoreApplication::applicationDirPath());
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName: entries) {
        const QString absoluteFilePath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader pluginLoader(absoluteFilePath);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            if (nitro::NitroModule *nitroModule = qobject_cast<nitro::NitroModule *>(plugin)) {
                nitroModule->registerDataTypes();
                const std::vector<nitro::CreatorVariant> nodes = nitroModule->registerNodes();
                RegisterNodes(node_registry, nodes, window);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(NITRO_NAME);
    QApplication::setApplicationVersion(NITRO_VERSION);
    setupApplication();

    auto node_registry = std::make_shared<nitro::NodeRegistry>();
    auto *window = new nitro::MainWindow();

    std::vector<std::unique_ptr<nitro::NitroModule>> modules = nitro::initModules();
    for (const auto &nitroModule: modules) {
        nitroModule->registerDataTypes();
        const auto nodes = nitroModule->registerNodes();
        RegisterNodes(*node_registry, nodes, *window);
    }
    LoadPlugins(*node_registry, *window);

    window->finalizeSetup(node_registry);
    window->show();
    return QApplication::exec();
}
