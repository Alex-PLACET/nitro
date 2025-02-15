#include "nitro/core/nodes/nitronodebuilder.hpp"

#include "QtNodes/internal/ConvertersRegister.hpp"
#include "nitro/core/nodes/datatypes/decimaldata.hpp"
#include "nitro/core/nodes/datatypes/integerdata.hpp"
#include "nitro/core/nodes/datatypes/pathdata.hpp"
#include "nitro/core/nodes/nitronode.hpp"
#include "nitro/util/imgresourcereader.hpp"

#include <QAbstractItemView>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtNodes/NodeColors.hpp>
#include <doubleslider.hpp>
#include <filesystem>
#include <intslider.hpp>
#include <utility>

using namespace nitro;

using DoubleSlider = ValueSliders::DoubleSlider;
using IntSlider = ValueSliders::IntSlider;

static QWidget *createWrapper() {
    auto *wrapper = new QWidget();
    wrapper->setContentsMargins(0, 0, 0, 0);
    auto *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    wrapper->setLayout(layout);
    return wrapper;
}

NitroNodeBuilder::NitroNodeBuilder(QString name, QString id, QString category)
    : name_(std::move(name)),
      id_(std::move(id)),
      category_(std::move(category)) {
    constexpr int portSpacing = 4;
    portWidgetHeight_ = QFontMetrics(QFont()).height() + 10;

    optionLayout_ = new QVBoxLayout();
    optionLayout_->setSpacing(portSpacing);
    optionLayout_->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    inLayout_ = new QVBoxLayout();
    inLayout_->setSpacing(portSpacing);
    inLayout_->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    outLayout_ = new QVBoxLayout();
    outLayout_->setSpacing(portSpacing);
    outLayout_->setAlignment(Qt::AlignTop | Qt::AlignRight);

    node_ = std::make_unique<NitroNode>();
}

std::unique_ptr<NitroNode> NitroNodeBuilder::build(
        const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
    QtNodes::NodeInfo info(name_, id_, category_, nodeColor_, iconPath_);
    QtNodes::NodeColors::registerColor(info);
    NodePorts nodePorts(inputList_, outputList_, options_, converters_register);
    auto *displayWrapper = new QWidget();
    displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
    auto *vLayout = new QVBoxLayout(displayWrapper);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    if (outLayout_->count() > 0) {
        auto *outWrapper = new QWidget();
        outWrapper->setLayout(outLayout_);
        vLayout->addWidget(outWrapper);
    }

    if (optionLayout_->count() > 0) {
        auto *optionWrapper = new QWidget();
        optionWrapper->setLayout(optionLayout_);
        vLayout->addWidget(optionWrapper);
    }

    if (inLayout_->count() > 0) {
        auto *inWrapper = new QWidget();
        inWrapper->setLayout(inLayout_);
        vLayout->addWidget(inWrapper);
    }

    node_->init(std::move(info), std::move(nodePorts), std::move(algo_), displayWrapper);
    return std::move(node_);
}

void NitroNodeBuilder::addOptionWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    optionLayout_->addWidget(widget, 0, Qt::AlignBottom | Qt::AlignLeft);
}

void NitroNodeBuilder::addInPortWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    inLayout_->addWidget(widget, 0, Qt::AlignBottom | Qt::AlignLeft);
}

void NitroNodeBuilder::addInPortWidget(QLabel *label) {
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    inLayout_->addWidget(label, 0, Qt::AlignBottom | Qt::AlignLeft);
}

void NitroNodeBuilder::addOutPortWidget(QWidget *widget) {
    widget->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    outLayout_->addWidget(widget, 0, Qt::AlignTop | Qt::AlignRight);
}

void NitroNodeBuilder::addOutPortWidget(QLabel *label) {
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setFixedSize(portWidgetWidth_, portWidgetHeight_);
    outLayout_->addWidget(label, 0, Qt::AlignTop | Qt::AlignRight);
}

NitroNodeBuilder *NitroNodeBuilder::withInputInteger(const QString &name,
                                                     int defaultVal,
                                                     int min,
                                                     int max,
                                                     BoundMode boundMode) {
    initInputValue(name, new IntSlider(name, defaultVal, min, max, boundMode));
    return this;
}

void NitroNodeBuilder::initInputValue(const QString &name, ValueSliders::IntSlider *slider) {
    const auto data = std::make_shared<IntegerData>(slider->getVal());
    inputList_.emplace_back(name, data);
    auto valueLabel = new QLabel(name);
    node_->connectInputWidget(slider, valueLabel, inputList_.size() - 1);
    QWidget *wrapper = createWrapper();
    wrapper->layout()->addWidget(slider);
    wrapper->layout()->addWidget(valueLabel);
    valueLabel->setHidden(true);
    addInPortWidget(wrapper);
}

void NitroNodeBuilder::initInputVal(const QString &name, ValueSliders::DoubleSlider *slider) {
    const auto data = std::make_shared<DecimalData>(slider->getVal());
    inputList_.emplace_back(name, data);
    auto valueLabel = new QLabel(name);
    node_->connectInputWidget(slider, valueLabel, inputList_.size() - 1);
    QWidget *wrapper = createWrapper();
    wrapper->layout()->addWidget(slider);
    wrapper->layout()->addWidget(valueLabel);
    valueLabel->setHidden(true);
    addInPortWidget(wrapper);
}

NitroNodeBuilder *NitroNodeBuilder::withInputValue(const QString &name,
                                                   double defaultVal,
                                                   double min,
                                                   double max,
                                                   BoundMode boundMode) {
    initInputVal(name, new DoubleSlider(name, defaultVal, min, max, boundMode));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withLoadButton(const QString &name,
                                                   const QString &filters,
                                                   const QString &outputName) {
    outputList_.emplace_back(name, std::make_shared<PathData>());
    // TODO: align icon
    auto *loadButton = new QPushButton();
    loadButton->setIcon(nitro::ImResourceReader::getPixMap(":/icons/folder_open.png"));
    loadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    loadButton->adjustSize();
    node_->connectLoadButton(name, loadButton, static_cast<int>(outputList_.size() - 1), filters);
    addOutPortWidget(new QLabel(outputName));
    addOptionWidget(loadButton);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name) {
    return withOutputInteger(name, 0);
}

NitroNodeBuilder *NitroNodeBuilder::withOutputInteger(const QString &name, int defaultVal) {
    outputList_.emplace_back(name, std::make_shared<IntegerData>(defaultVal));
    addOutPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name) {
    return withOutputValue(name, 0);
}

NitroNodeBuilder *NitroNodeBuilder::withOutputValue(const QString &name, double defaultVal) {
    outputList_.emplace_back(name, std::make_shared<DecimalData>(defaultVal));
    addOutPortWidget(new QLabel(name));
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputInteger(const QString &name,
                                                             int defaultVal,
                                                             int min,
                                                             int max,
                                                             BoundMode boundMode) {
    outputList_.emplace_back(name, std::make_shared<IntegerData>(defaultVal));

    auto slider = new IntSlider("", defaultVal, min, max, boundMode);
    node_->connectSourceInteger(slider, outputList_.size() - 1);
    addOutPortWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withSourcedOutputValue(const QString &name,
                                                           double defaultVal,
                                                           double min,
                                                           double max,
                                                           BoundMode boundMode) {
    outputList_.emplace_back(name, std::make_shared<DecimalData>(defaultVal));

    auto slider = new DoubleSlider("", defaultVal, min, max, boundMode);
    node_->connectSourceValue(slider, outputList_.size() - 1);
    addOutPortWidget(slider);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDropDown(const QString &name, const QStringList &options) {
    auto *comboBox = new QComboBox();
    comboBox->addItems(options);

    // TODO: ensure scrollbar for too many items
    options_[name] = comboBox->currentIndex();
    node_->connectComboBox(name, comboBox);
    addOptionWidget(comboBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withCheckBox(const QString &name, bool checked) {
    auto *checkBox = new QCheckBox(name);
    checkBox->setChecked(checked);
    options_[name] = checkBox->isChecked();
    node_->connectCheckBox(name, checkBox);
    addOptionWidget(checkBox);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDisplayWidget(const QString &name, QLabel *label) {
    node_->connectLabel(name, label);
    addOptionWidget(label);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withDisplayWidget(const QString &name, const QString &text) {
    auto *label = new QLabel(text);
    node_->connectLabel(name, label);
    addOptionWidget(label);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withNodeColor(const QColor &color) {
    nodeColor_ = color;
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withIcon(const QString &path) {
    iconPath_ = QString(":/icons/nodes/%1").arg(path);
    return this;
}

NitroNodeBuilder *NitroNodeBuilder::withOperator(std::unique_ptr<NodeOperator> op) {
    algo_ = std::move(op);
    return this;
}
