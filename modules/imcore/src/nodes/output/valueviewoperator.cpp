#include "valueviewoperator.hpp"

#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/nitronodebuilder.hpp>


namespace nitro::ImCore {

static inline const QString DISPLAY_LABEL = "valueLabel";
static inline const QString INPUT_VALUE = "Value";

ValueViewOperator::ValueViewOperator(QLabel *valueLabel) : valueLabel_(valueLabel) {}

void ValueViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    const double val = nodePorts.inGetAs<DecimalData>(INPUT_VALUE);
    double intPart = 0;
    if (std::modf(val, &intPart) == 0.0) {
        valueLabel_->setText(QString::number(val));
    } else {
        valueLabel_->setText(QString::number(val, 'f', 3));
    }
}

CreatorWithoutParameters ValueViewOperator::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        NitroNodeBuilder builder("Value Display", "valueViewer", category);
        auto *valueLabel = new QLabel("-");
        return builder.withOperator(std::make_unique<ValueViewOperator>(valueLabel))
                ->withIcon("number.png")
                ->withDisplayWidget(DISPLAY_LABEL, valueLabel)
                ->withNodeColor(NITRO_OUTPUT_COLOR)
                ->withInputPort<DecimalData>(INPUT_VALUE)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
