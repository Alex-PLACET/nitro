#include "nitro/modules/imcore.hpp"

#include "nodes/color/colormap.hpp"
#include "nodes/color/colorspaceconvert.hpp"
#include "nodes/color/immix.hpp"
#include "nodes/color/uniformconvert.hpp"
#include "nodes/converter/booleanmath.hpp"
#include "nodes/converter/combine.hpp"
#include "nodes/converter/iminfo.hpp"
#include "nodes/converter/immath.hpp"
#include "nodes/converter/invert.hpp"
#include "nodes/converter/maprange.hpp"
#include "nodes/converter/normalize.hpp"
#include "nodes/converter/reduction.hpp"
#include "nodes/converter/rgbtobw.hpp"
#include "nodes/converter/separate.hpp"
#include "nodes/input/imagesourceoperator.hpp"
#include "nodes/input/inputPath.hpp"
#include "nodes/input/randomoperator.hpp"
#include "nodes/input/rgbinput.hpp"
#include "nodes/output/jsonviewoperator.hpp"
#include "nodes/output/valueviewoperator.hpp"
#include "nodes/transform/imtranslate.hpp"
#include "nodes/transform/matchsize.hpp"
#include "src/nodes/output/histogramviewoperator.hpp"
#include "src/nodes/output/imageviewoperator.hpp"
#include "src/nodes/output/jsonviewoperator.hpp"
#include "src/nodes/transform/imflip.hpp"
#include "src/nodes/transform/imresize.hpp"
#include "src/nodes/transform/imrotate.hpp"

#include <QtNodes/ConvertersRegister>
#include <nitro/core/nodes/datatypes/decimaldata.hpp>
#include <nitro/core/nodes/datatypes/integerdata.hpp>
#include <nitro/core/nodes/noderegistry.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>

namespace nitro::ImCore {

std::vector<CreatorVariant> ImCore::registerNodes() {
    std::vector<CreatorVariant> node_creators;
    registerInputNodes(node_creators);
    registerOutputNodes(node_creators);
    registerConvertNodes(node_creators);
    registerColorNodes(node_creators);
    registerTransformNodes(node_creators);
    return node_creators;
}

QtNodes::ConvertersRegister ImCore::registerDataTypes() {
    QtNodes::ConvertersRegister converters_register;
    // converters_register.merge(imcore::Get_ColImageData_conversions());
    // converters_register.merge(imcore::Get_GrayImageData_conversions());
    return converters_register;
}

void ImCore::registerConvertNodes(std::vector<CreatorVariant> &node_creators) {
    const QString category = "Converter";
    node_creators.emplace_back(ImInfoOperator::creator(category)); // TODO: place elsewhere?
    node_creators.emplace_back(MathOperator::creator(category));
    node_creators.emplace_back(BooleanMathOperator::creator(category));
    node_creators.emplace_back(ReductionOperator::creator(category));
    node_creators.emplace_back(MapRangeOperator::creator(category));
    node_creators.emplace_back(NormalizeOperator::creator(category));
    node_creators.emplace_back(InvertOperator::creator(category));
    node_creators.emplace_back(RgbToGrayscaleOperator::creator(category));
    node_creators.emplace_back(SeparateOperator::creator(category));
    node_creators.emplace_back(CombineOperator::creator(category));
}

void ImCore::registerTransformNodes(std::vector<CreatorVariant> &node_creators) {
    const QString category = "Transform";
    node_creators.emplace_back(ResizeOperator::creator(category));
    node_creators.emplace_back(ImFlipOperator::creator(category));
    node_creators.emplace_back(ImRotateOperator::creator(category));
    node_creators.emplace_back(TranslateOperator::creator(category));
    node_creators.emplace_back(MatchSizeOperator::creator(category));
}

void ImCore::registerColorNodes(std::vector<CreatorVariant> &node_creators) {
    const QString category = "Color";
    node_creators.emplace_back(MixOperator::creator(category));
    node_creators.emplace_back(ConvertOperator::creator(category));
    node_creators.emplace_back(UniformConvertOperator::creator(category));
    node_creators.emplace_back(ColorMapOperator::creator(category));
}

void ImCore::registerInputNodes(std::vector<CreatorVariant> &node_creators) {
    const QString category = "Input";

    // ------ Image Source Node ------
    node_creators.emplace_back(ImageSourceOperator::creator(category));

    node_creators.emplace_back(InputPath::creator(category));

    // ------ Decimal Source Node ------
    node_creators.emplace_back([category](const std::shared_ptr<const QtNodes::ConvertersRegister>
                                                  &converters_register) {
        NitroNodeBuilder builder("Value", "ValueSource", category);
        return builder.withSourcedOutputValue("Value", 0, 0, 1, BoundMode::UNCHECKED)
                ->withIcon("number.png")
                ->withNodeColor(NITRO_INPUT_COLOR)
                ->build(converters_register);
    });

    // ------ Integer Source Node ------
    node_creators.emplace_back([category](const std::shared_ptr<const QtNodes::ConvertersRegister>
                                                  &converters_register) {
        NitroNodeBuilder builder("Integer", "IntegerSource", category);
        return builder.withSourcedOutputInteger("Integer", 128, 0, 255, BoundMode::UNCHECKED)
                ->withIcon("number.png")
                ->withNodeColor(NITRO_INPUT_COLOR)
                ->build(converters_register);
    });
    node_creators.emplace_back(RandomOperator::creator(category));
    node_creators.emplace_back(RgbOperator::creator(category));
}

void ImCore::registerOutputNodes(std::vector<CreatorVariant> &node_creators) {
    const QString category = "Output";
    node_creators.emplace_back([category](MainWindow *window) {
        return ImageViewOperator::creator(category, window);
    });
    node_creators.emplace_back(ValueViewOperator::creator(category));
    node_creators.emplace_back([category](MainWindow *window) {
        return HistogramViewOperator::creator(category, window);
    });
    node_creators.emplace_back([category](MainWindow *window) {
        return JsonViewOperator::creator(category, window);
    });
}

} // namespace nitro::ImCore
