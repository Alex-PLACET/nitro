#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QSplitter>

namespace nitro {

class NodeRegistry;

class NodeDockWidget;

/**
 * @brief The main window of the application. Contains the node editor, GUI dock widgets, file menu and footer.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Initialises a new empty MainWindow.
     * @param parent Parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow() override;

    /**
     * @brief Creates an icon for a GUI dock widget of a specific size and color.
     * @param path Path of the icon as defined in the relevant resources.qrc.
     * @return A QLabel having the icon as its pixmap.
     */
    QLabel *buildDockIcon(const QString &path);

    /**
     * @brief Registers a GUI dock widget. A toggle for the dock widget is added to the window menu.
     * @param widget The new dock widget.
     */
    void registerDock(QDockWidget *widget);

    /**
     * @brief Registers a new widget that displays the node canvas/graph. Note that NITRO currently only supports a single NodeDockWidget at the time.
     * @param widget The NodeDockWidget to register.
     */
    void registerNodeDock(NodeDockWidget *widget);

    /**
     * @brief Finalizes the setup for this window. Should always be called once everything has been initialised (i.e. all docks have been registered).
     * @param registry Node registry containing the supported nodes and data types.
     * Note that once a MainWindow is finalized, adding new nodes to the node registry does not change anything in the MainWindow.
     */
    void finalizeSetup(std::shared_ptr<NodeRegistry> &registry);

    /**
     * @brief Checks whether the window is finalized or not.
     * @return True if the MainWindow is finalized. False otherwise.
     */
    [[nodiscard]] bool isFinalized() const;

private:
    const int icSize_ = 16;
    const QColor icColor_ = {148, 148, 148};
    std::vector<QDockWidget *> widgets_;
    QLabel *fileNameLabel_ = nullptr;
    NodeDockWidget *nodeDock_ = nullptr;
    QSplitter *dockLayout_ = nullptr;
    QSplitter *vertLayout_;

    QMenuBar *initMenuBar();

    QStatusBar *initFooter();

    QMenu *getFileMenu();

    QMenu *getWindowMenu();

    bool finalized_ = false;
};

} // namespace nitro
