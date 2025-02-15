#pragma once

#include <QGraphicsView>
#include <QLabel>
#include <QMenu>
#include <QOpenGLDebugLogger>
#include <QScrollArea>
#include <QVector3D>
#include <opencv2/core/mat.hpp>

namespace nitro::ImCore {

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    struct ScaleRange {
        double minimum = 0;
        double maximum = 0;
    };

public:
    explicit ImageViewer(QGraphicsScene *imScene, QWidget *parent = nullptr);

    ~ImageViewer() override;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void resetImScale();

    void setImage(const std::shared_ptr<cv::Mat> &img);

    void drawBackground(QPainter *painter, const QRectF &r) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void removeImage();

    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void drawForeground(QPainter *painter, const QRectF &rect) override;

    const double minScaleFactor = 0.2;
    const double maxScaleFactor = 20;

protected:
    void saveImage();

public Q_SLOTS:

    void scaleUp();

    void scaleDown();

Q_SIGNALS:

    void scaleChanged(double scale);

    void imageUpdated(const cv::Mat &img);

    void mousePosUpdated(const QPoint &pos, QColor color);

private:
    const int dotSize_ = 3;
    const int gridStep_ = 64;

    const int gridStepSize_ = 32;
    const int emptySize_ = 128;

    ScaleRange scaleRange_;
    QImage displayImg_;
    cv::Mat currentImg_;
    QPoint itemPos_;
    QGraphicsPixmapItem *imgDisplayItem_ = nullptr;
    QAction *saveAction_;
    QAction *resetAction_;
    bool removalDue_ = false;
    bool crossHairMode_ = false;

    void setScaleRange(double minimum = 0, double maximum = 0);

    void setupScale(double scale);

    QMenu *createContextMenu();

    void centerScene();

    void initActions();

    QString lastFilePath_ = "../data/";

    void drawFooter(QPainter *painter) const;
};

} // namespace nitro::ImCore
