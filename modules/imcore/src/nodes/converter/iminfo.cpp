#include "iminfo.hpp"

#include "nitro/datatypes/colimagedata.hpp"

#include <nitro/core/modules/nitromodule.hpp>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>
#include <nitro/util/util.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static const QString INPUT_IMAGE(QStringLiteral("Image"));
static const QString OUTPUT_WIDTH(QStringLiteral("Width"));
static const QString OUTPUT_HEIGHT(QStringLiteral("Height"));
static const QString OUTPUT_AR(QStringLiteral("Aspect Ratio"));
static const QString OUTPUT_NUM_PIXELS(QStringLiteral("Num Pixels"));
static const QString OUTPUT_TYPE(QStringLiteral("Type"));

ImInfoOperator::ImInfoOperator(QLabel *typeLabel) : typeLabel_(typeLabel) {}

void ImInfoOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    nodePorts.output<IntegerData>(OUTPUT_WIDTH, im1->cols);
    nodePorts.output<IntegerData>(OUTPUT_HEIGHT, im1->rows);
    nodePorts.output<IntegerData>(OUTPUT_NUM_PIXELS, im1->cols * im1->rows);
    nodePorts.output<DecimalData>(OUTPUT_AR, double(im1->cols) / double(im1->rows));

    QString type;
    switch (im1->depth()) {
        case CV_8U: // 8-bit unsigned integer (0-255)
            type = "8-bit uint";
            break;
        case CV_8S: // 8-bit signed integer (-128 to 127)
            type = "8-bit int";
            break;
        case CV_16U: // 16-bit unsigned integer (0-65535)
            type = "16-bit uint";
            break;
        case CV_16S: // 16-bit signed integer (-32768 to 32767)
            type = "16-bit int";
            break;
        case CV_32S: // 32-bit signed integer
            type = "32-bit int";
            break;
        case CV_32F: // 32-bit floating-point
            type = "Float";
            break;
        case CV_64F: // 64-bit floating-point
            type = "Double";
            break;
        default:
            type = "-";
            break;
    }

    typeLabel_->setText(QString("Type: %1").arg(type));
}

CreatorWithoutParameters ImInfoOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Image Info", "imInfo", category);
        auto *typeLabel = new QLabel("Type: ");
        return builder.withOperator(std::make_unique<ImInfoOperator>(typeLabel))
                ->withIcon("info.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withDisplayWidget(OUTPUT_TYPE, typeLabel)
                ->withOutputInteger(OUTPUT_WIDTH)
                ->withOutputInteger(OUTPUT_HEIGHT)
                ->withOutputInteger(OUTPUT_NUM_PIXELS)
                ->withOutputValue(OUTPUT_AR)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
