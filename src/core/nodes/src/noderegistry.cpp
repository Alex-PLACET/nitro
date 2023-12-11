#include "nitro/core/nodes/noderegistry.hpp"

#include "nitro/core/nodes/datatypes/decimaldata_conversions.hpp"
#include "nitro/core/nodes/nitronode.hpp"

nitro::NodeRegistry::NodeRegistry() {
    registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    nitro::Get_DecimalData_conversions();
}

const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &nitro::NodeRegistry::getRegistry()
        const {
    return registry_;
}

std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> nitro::NodeRegistry::getCategories()
        const {
    std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> categories;
    categories.resize(categoryOrder_.size());
    for (const auto &[key, value]: categoryOrder_) {
        categories[value] = {key, categories_.at(key)};
    }
    return categories;
}

void nitro::NodeRegistry::registerNode(const CreatorWithoutParameters &buildFunction) {
    QtNodes::NodeInfo info = buildFunction(registry_->convertersRegister())->getInfo();
    const QString &category = info.getCategory();
    if (!categoryOrder_.contains(category)) {
        categoryOrder_[category] = catIdx_;
        catIdx_++;
    }
    categories_[category].emplace_back(std::move(info));
    registry_->registerModel<nitro::NitroNode>(buildFunction);
}
