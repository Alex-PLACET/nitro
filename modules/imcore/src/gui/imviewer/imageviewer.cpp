#include "imageviewer.hpp"

#include <QApplication>
#include <QColorSpace>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>
#include <QtAlgorithms>
#include <cmath>
#include <nitro/util/util.hpp>


namespace nitro::ImCore {

ImageViewer::ImageViewer(QGraphicsScene *imScene, QWidget *parent) : QGraphicsView(parent) {
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    setResizeAnchor(QGraphicsView::transformationAnchor());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setScaleRange(minScaleFactor, maxScaleFactor);

    // Sets the scene rect to its maximum possible ranges to avoid auto scene range
    // re-calculation when expanding the all QGraphicsItems core rect.
    constexpr int maxSize = 14960;
    setSceneRect(-maxSize, -maxSize, (maxSize * 2), (maxSize * 2));
    setScene(imScene);
    resetImScale();
    initActions();
}

ImageViewer::~ImageViewer() = default;

void ImageViewer::initActions() {
    resetAction_ = new QAction("Reset view", this);
    connect(resetAction_, &QAction::triggered, this, &ImageViewer::resetImScale);

    saveAction_ = new QAction("Save image", this);
    connect(saveAction_, &QAction::triggered, this, &ImageViewer::saveImage);
}

void ImageViewer::saveImage() {

    if (displayImg_.sizeInBytes() > 0) {
        QString filePath = QFileDialog::getSaveFileName(
                this,
                "Save Image",
                lastFilePath_,
                tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));
        if (filePath == "") {
            return;
        }
        lastFilePath_ = filePath;
        if (displayImg_.save(filePath)) {
            QMessageBox::information(this,
                                     tr("Save Successful"),
                                     QString("File saved to\n %1").arg(filePath));
        } else {
            QMessageBox::warning(
                    this,
                    tr("Could not save"),
                    QString("Something went wrong while trying to save to\n %1").arg(filePath));
        }
    }
}

void ImageViewer::drawBackground(QPainter *painter, const QRectF &r) {

    const QColor dotColor_ = palette().color(QPalette::Button);
    const QColor gridBackgroundColor_ = palette().color(QPalette::Disabled,
                                                        QPalette::AlternateBase);
    const QColor bGroundCol_ = palette().color(QPalette::Base);
    const QColor imgOutlineCol_ = palette().color(QPalette::Disabled, QPalette::Button);
    const QColor imgGridCol_ = palette().color(QPalette::Base);

    painter->setBrush(QBrush(bGroundCol_));
    QGraphicsView::drawBackground(painter, r);
    painter->setBrush(QBrush(dotColor_));
    painter->setPen(dotColor_);

    QRect windowRect = rect();
    QPointF tl = mapToScene(windowRect.topLeft());
    QPointF br = mapToScene(windowRect.bottomRight());

    double left = std::floor(tl.x() / gridStep_ - 0.5);
    double right = std::floor(br.x() / gridStep_ + 1.0);
    double bottom = std::floor(tl.y() / gridStep_ - 0.5);
    double top = std::floor(br.y() / gridStep_ + 1.0);

    for (int xi = static_cast<int>(left); xi <= static_cast<int>(right); ++xi) {
        for (int yi = static_cast<int>(bottom); yi <= static_cast<int>(top); ++yi) {
            painter->drawEllipse(xi * gridStep_, yi * gridStep_, dotSize_, dotSize_);
        }
    }

    if (imgDisplayItem_ == nullptr) {
        QRectF gridRect(-emptySize_, -emptySize_, emptySize_ * 2, emptySize_ * 2);
        QPen pBounds(imgOutlineCol_, 2.0);
        painter->setBrush(QBrush(gridBackgroundColor_));
        painter->drawRect(gridRect);

        QPen pFine(imgGridCol_, 1.0);

        painter->setPen(pFine);
        for (qreal x = gridRect.x() + gridStepSize_; x < gridRect.x() + gridRect.width();
             x += gridStepSize_) {
            painter->drawLine(x, gridRect.y(), x, gridRect.y() + gridRect.height());
        }
        for (qreal y = gridRect.y() + gridStepSize_; y < gridRect.y() + gridRect.height();
             y += gridStepSize_) {
            painter->drawLine(gridRect.x(), y, gridRect.x() + gridRect.width(), y);
        }

        painter->setPen(pBounds);
        QBrush brush(Qt::transparent);
        painter->setBrush(brush);
        painter->drawRect(gridRect);
    }
}

void ImageViewer::drawFooter(QPainter *painter) const {
    const double footerHeight = 30;
    const QRectF &r = geometry();
    QRectF footerRect(0, r.height() - footerHeight, r.width(), footerHeight);

    // Draw the footer background
    QColor footerBackgroundColor = QColor(0, 0, 0, 80); // RGB(64, 64, 64) with alpha 200
    painter->setBrush(QBrush(footerBackgroundColor));
    painter->setPen(Qt::NoPen);
    painter->drawRect(footerRect);

    painter->setPen(Qt::white);

    QColor color;
    if (displayImg_.rect().contains(itemPos_)) {
        color = displayImg_.pixelColor(itemPos_);
    } else {
        color = {0, 0, 0};
    }

    // Monospaced font
    QString monospacedFontFamily = "Monospace";
    QFont monospacedFont(monospacedFontFamily);
    monospacedFont.setStyleHint(QFont::TypeWriter);
    painter->setFont(monospacedFont);

    QString xValue = QString::number(itemPos_.x());
    QString paddedXValue = xValue.rightJustified(5, ' ');

    QString yValue = QString::number(itemPos_.y());
    QString paddedYValue = yValue.rightJustified(5, ' ');

    QString footerText = "  X:" + paddedXValue + "  Y:" + paddedYValue + "  |" +
                         "  R:" + QString::number(color.redF(), 'f', 3) +
                         "  G:" + QString::number(color.greenF(), 'f', 3) +
                         "  B:" + QString::number(color.blueF(), 'f', 3) + "  |" +
                         "  H:" + QString::number(color.hueF(), 'f', 3) +
                         "  S:" + QString::number(color.saturationF(), 'f', 3) +
                         "  V:" + QString::number(color.valueF(), 'f', 3) +
                         "  L:" + QString::number(color.lightnessF(), 'f', 3) + "  |" + "  " +
                         color.name();
    painter->drawText(footerRect, Qt::AlignLeft | Qt::AlignVCenter, footerText);
}

void ImageViewer::setScaleRange(double minimum, double maximum) {
    if (maximum < minimum)
        std::swap(minimum, maximum);
    minimum = std::max(0.0, minimum);
    maximum = std::max(0.0, maximum);

    scaleRange_ = {minimum, maximum};

    setupScale(transform().m11());
}

void ImageViewer::scaleUp() {
    double const step = 1.2;
    double const factor = std::pow(step, 1.0);

    if (scaleRange_.maximum > 0) {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() >= scaleRange_.maximum) {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    Q_EMIT scaleChanged(transform().m11());
}

void ImageViewer::scaleDown() {
    double const step = 1.2;
    double const factor = std::pow(step, -1.0);

    if (scaleRange_.minimum > 0) {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() <= scaleRange_.minimum) {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    Q_EMIT scaleChanged(transform().m11());
}

void ImageViewer::setupScale(double scale) {
    scale = std::max(scaleRange_.minimum, std::min(scaleRange_.maximum, scale));

    if (scale <= 0)
        return;

    if (scale == transform().m11())
        return;

    QTransform matrix;
    matrix.scale(scale, scale);
    setTransform(matrix, false);

    Q_EMIT scaleChanged(scale);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    QPoint delta = event->angleDelta();

    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());

    if (d > 0.0)
        scaleUp();
    else
        scaleDown();
}

QMenu *ImageViewer::createContextMenu() {
    auto *menu = new QMenu();
    menu->addAction(resetAction_);
    menu->addAction(saveAction_);
    if (displayImg_.sizeInBytes() == 0) {
        saveAction_->setEnabled(false);
    }
    return menu;
}

void ImageViewer::contextMenuEvent(QContextMenuEvent *event) {
    QMenu *menu = createContextMenu();
    menu->exec(event->globalPos());
}

void ImageViewer::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
}

void ImageViewer::setImage(const std::shared_ptr<cv::Mat> &img) {
    if (img->empty()) {
        removeImage();
        return;
    }
    removalDue_ = false;

    int oldDisplayWidth = displayImg_.width();
    int oldDisplayHeight = displayImg_.height();

    displayImg_ = cvMatToQImage(*img, currentImg_); // TODO fix memory leak
    if (imgDisplayItem_ == nullptr) {
        imgDisplayItem_ = new QGraphicsPixmapItem(QPixmap::fromImage(displayImg_));
        scene()->addItem(imgDisplayItem_);
        QRectF rect = scene()->itemsBoundingRect();
        scene()->setSceneRect(rect);
        resetImScale();
    } else {
        imgDisplayItem_->setPixmap(QPixmap::fromImage(displayImg_));
        if (imgDisplayItem_->boundingRect().width() != oldDisplayWidth ||
            imgDisplayItem_->boundingRect().height() != oldDisplayHeight) {
            //            QRectF rect = scene()->itemsBoundingRect();
            //            scene()->setSceneRect(rect);
            //            resetImScale();
        }
    }
    emit imageUpdated(*img);
    repaint();
}

void ImageViewer::removeImage() {
    removalDue_ = true;
    auto *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(200);
    connect(timer, &QTimer::timeout, [this, timer]() {
        if (removalDue_) {
            if (imgDisplayItem_ != nullptr) {
                scene()->removeItem(imgDisplayItem_);
                imgDisplayItem_ = nullptr;
            }
            resetImScale();
            emit imageUpdated(cv::Mat());
            repaint();
        }
        timer->deleteLater();
    });

    timer->start();
}

void ImageViewer::centerScene() {
    if (scene()) {
        scene()->setSceneRect(QRectF());
        if (imgDisplayItem_) {
            centerOn(imgDisplayItem_->boundingRect().width() / 2,
                     imgDisplayItem_->boundingRect().height() / 2);
        } else {
            centerOn(0, 0);
        }
    }
}

void ImageViewer::resetImScale() {
    centerScene();
    if (imgDisplayItem_ == nullptr) {
        setupScale(1.0);
    } else {
        auto maxSize = std::max(imgDisplayItem_->boundingRect().width(),
                                imgDisplayItem_->boundingRect().height());
        auto minCurSize = std::min(rect().width(), rect().height());
        double scale = minCurSize / maxSize;
        setupScale(0.8 * scale);
    }
    centerScene();
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_R) {
        resetImScale();
        event->accept();
        return;
    } else if (event->key() == Qt::Key_S && event->modifiers().testFlag(Qt::AltModifier)) {
        QApplication::restoreOverrideCursor();
        saveImage();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Control) {
        if (!crossHairMode_ && rect().contains(mapFromGlobal(QCursor::pos()))) {
            crossHairMode_ = true;
            QApplication::setOverrideCursor(Qt::CrossCursor);
        }
    }
}

void ImageViewer::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsView::keyReleaseEvent(event);
    if (event->key() == Qt::Key_Control) {
        crossHairMode_ = false;
        QApplication::restoreOverrideCursor();
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    if (imgDisplayItem_ != nullptr && crossHairMode_) {
        QPointF scenePos = mapToScene(event->pos());
        if (!imgDisplayItem_->contains(scenePos)) {
            return;
        }
        itemPos_ = imgDisplayItem_->mapFromScene(scenePos).toPoint();
        scene()->update();
    }
}

void ImageViewer::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    if (crossHairMode_) {
        QApplication::restoreOverrideCursor();
        crossHairMode_ = false;
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
    if (!crossHairMode_) {
        QGraphicsView::mousePressEvent(event);
    }
}

void ImageViewer::drawForeground(QPainter *painter, const QRectF &r) {
    painter->resetTransform();
    drawFooter(painter);
}

} // namespace nitro::ImCore
