#pragma once

#include <QtNodes/ConvertersRegister>
#include <functional>
#include <nitro/core/nodes/nitronode.hpp>

namespace nitro {

class MainWindow;

using CreatorWithoutParameters = std::function<std::unique_ptr<NitroNode>(
        const std::shared_ptr<const QtNodes::ConvertersRegister> &)>;
using CreatorWithWindow = std::function<CreatorWithoutParameters(MainWindow *window)>;
using CreatorVariant = std::variant<CreatorWithWindow, CreatorWithoutParameters>;

} // namespace nitro
