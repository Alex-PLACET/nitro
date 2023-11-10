#include <qpluginloader.h>
#if _OPENMP
#include <omp.h>
#endif

#include "nitro/gui/nodeeditor/stylepresets.hpp"

#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QStyle>
#include <QSurfaceFormat>
#include <QtNodes/internal/WidgetNodePainter.hpp>
#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/noderegistry.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>
#include <nitro/modules/module_list.hpp>
#include <nitro/version.hpp>

template<class>
inline constexpr bool always_false_v = false;

void setupApplication() {
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4, 1);
    glFormat.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(glFormat);
#ifdef _OPENMP
    omp_set_num_threads(8);
    cv::setNumThreads(8);
#else
    std::cerr << "Warning: OpenMP not found. No parallelization available." << std::endl;
#endif
    nitro::applyStylePreset(nitro::getDarkModePalette());

    QFont font("Lato");
    font.setPixelSize(14);
    QApplication::setFont(font);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(NITRO_NAME);
    QApplication::setApplicationVersion(NITRO_VERSION);
    setupApplication();

    std::vector<std::unique_ptr<NitroModule>> modules = nitro::initModules();

    auto node_registry = std::make_shared<nitro::NodeRegistry>();
    auto *window = new nitro::MainWindow();
    for (const auto &nitroModule: modules) {
        nitroModule->registerDataTypes();
        const auto nodes = nitroModule->registerNodes();
        for (const auto &node: nodes) {
            std::visit(
                    [&node_registry, window](auto &&node_arg) {
                        using T = std::decay_t<decltype(node_arg)>;
                        if constexpr (std::is_same_v<T, CreatorWithWindow>)
                            node_registry->registerNode(node_arg(window));
                        else if constexpr (std::is_same_v<T, CreatorWithoutParameters>)
                            node_registry->registerNode(node_arg);
                        else
                            static_assert(always_false_v<T>, "non-exhaustive visitor!");
                    },
                    node);
        }
    }

    QDir pluginsDir(QCoreApplication::applicationDirPath());
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName: entries) {
        const QString absoluteFilePath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader pluginLoader(absoluteFilePath);
        const auto metadata = pluginLoader.metaData();
        QObject *plugin = pluginLoader.instance();
        QString lol = pluginLoader.errorString();
        if (plugin) {
            NitroModule *nitroModule = qobject_cast<NitroModule *>(plugin);
            if (nitroModule) {
                nitroModule->registerDataTypes();
                const auto nodes = nitroModule->registerNodes();
                for (const auto &node: nodes) {
                    std::visit(
                            [&node_registry, window](auto &&node_arg) {
                                using T = std::decay_t<decltype(node_arg)>;
                                if constexpr (std::is_same_v<T, CreatorWithWindow>)
                                    node_registry->registerNode(node_arg(window));
                                else if constexpr (std::is_same_v<T, CreatorWithoutParameters>)
                                    node_registry->registerNode(node_arg);
                                else
                                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
                            },
                            node);
                }
            }
            pluginLoader.unload();
        }
    }
    QPluginLoader loader("libnitro_gui_nodeeditor.so");

    window->finalizeSetup(node_registry);
    window->show();
    return QApplication::exec();
}
