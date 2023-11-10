#include "nitro/modules/improc.hpp"

#include "nodes/analysis/distancetransform.hpp"
#include "nodes/blur/bilateralfilter.hpp"
#include "nodes/blur/boxfilter.hpp"
#include "nodes/blur/gaussianblur.hpp"
#include "nodes/filter/canny.hpp"
#include "nodes/filter/dctransform.hpp"
#include "nodes/filter/fouriershift.hpp"
#include "nodes/filter/fouriertransform.hpp"
#include "nodes/input/mask.hpp"
#include "nodes/input/structuringelement.hpp"
#include "nodes/quality/flip.hpp"
#include "nodes/quality/mse.hpp"
#include "nodes/quality/psnr.hpp"
#include "nodes/quantization/kmeans.hpp"
#include "nodes/quantization/quantize.hpp"
#include "nodes/restoration/denoise.hpp"
#include "nodes/segmentation/connectedcomps.hpp"
#include "src/nodes/filter/morphology.hpp"

#include <nitro/core/nodes/noderegistry.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>


namespace nitro::ImProc {

ImProc::ImProc() = default;

std::vector<CreatorVariant> ImProc::registerNodes() {
    std::vector<CreatorVariant> node_creators;
    registerFilterNodes(node_creators);
    registerAnalysisNodes(node_creators);
    registerInputNodes(node_creators);
    registerQualityMetricNodes(node_creators);
    registerRestorationNodes(node_creators);
    registerQuantizationNodes(node_creators);
    return node_creators;
}

void ImProc::registerInputNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Input";
    node_creator.emplace_back(MaskOperator::creator(category));
    node_creator.emplace_back(StructElemOperator::creator(category));
}

void ImProc::registerFilterNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Filter";
    node_creator.emplace_back(BoxFilterOperator::creator(category));
    node_creator.emplace_back(GaussianBlurOperator::creator(category));
    node_creator.emplace_back(BilateralFilterOperator::creator(category));
    node_creator.emplace_back(CannyEdgeDetectionOperator::creator(category));
    node_creator.emplace_back(MorphologyOperator::creator(category));
    node_creator.emplace_back(FFTOperator::creator(category));
    node_creator.emplace_back(DCTOperator::creator(category));
    node_creator.emplace_back(FFTShiftOperator::creator(category));
}

void ImProc::registerAnalysisNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Analysis";
    node_creator.emplace_back(DistanceTransformOperator::creator(category));
    node_creator.emplace_back(ConnectedCompsOperator::creator(category));
}

void ImProc::registerQualityMetricNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Quality";
    node_creator.emplace_back(MseOperator::creator(category));
    node_creator.emplace_back(PsnrOperator::creator(category));
    // registry->registerNode(FlipOperator::creator(category));
}

void ImProc::registerRestorationNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Restoration";
    node_creator.emplace_back(DenoiseOperator::creator(category));
}

void ImProc::registerQuantizationNodes(std::vector<CreatorVariant> &node_creator) const {
    const QString category = "Quantization";
    node_creator.emplace_back(QuantizeOperator::creator(category));
    node_creator.emplace_back(KMeansOperator::creator(category));
}

} // namespace nitro::ImProc
