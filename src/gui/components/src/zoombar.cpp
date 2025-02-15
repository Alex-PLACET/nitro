#include "nitro/gui/components/zoombar.hpp"

namespace nitro {

ZoomBar::ZoomBar(int minZoom, int maxZoom, QWidget *parent)
    : QProgressBar(parent),
      curZoom_(100),
      minZoom_(minZoom),
      maxZoom_(maxZoom) {
    setValue(minimum());
}

QString ZoomBar::text() const {
    return QString::number(curZoom_) + "%";
}

void ZoomBar::setZoom(int zoom) {
    curZoom_ = zoom;
    const double logValue = log10(curZoom_);
    const double logMin = log10(minZoom_);
    const double logMax = log10(maxZoom_);
    const double logPercent = (logValue - logMin) / (logMax - logMin);
    update();
}

void ZoomBar::setZoom(double zoomFactor) {
    setZoom(int(zoomFactor * 100.0 + 0.5));
}

} // namespace nitro
