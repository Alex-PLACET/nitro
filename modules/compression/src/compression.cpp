#include "nitro/modules/compression.hpp"

#include "nodes/compression/jpegoperator.hpp"
#include "nodes/compression/pngoperator.hpp"
#include "nodes/compression/zliboperator.hpp"

#include <nitro/core/nodes/noderegistry.hpp>
#include <nitro/gui/nodeeditor/mainwindow.hpp>

namespace nitro::Compression {

Compression::Compression() = default;

std::vector<CreatorVariant> Compression::registerNodes() {
    std::vector<CreatorVariant> node_creators;
    registerCompressionNodes(node_creators);
    return node_creators;
}

void Compression::registerCompressionNodes(std::vector<CreatorVariant> &node_creators) const {
    const QString category = "Compression";
    node_creators.emplace_back(ZLibOperator::creator(category));
    node_creators.emplace_back(PngOperator::creator(category));
    node_creators.emplace_back(JpegOperator::creator(category));
}

} // namespace nitro::Compression
