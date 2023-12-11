#include "inputPath.hpp"

#include "nitro/core/nodes/datatypes/pathdata.hpp"

#include <filesystem>
#include <nitro/core/nodes/nitronodebuilder.hpp>

namespace nitro::ImCore {

// static inline const QString LOAD_PATH = "LoadPath";
static inline const QString OUTPUT_PATH = "Path";

InputPath::InputPath(QLabel *displayPathLabel) : displayPathLabel_(displayPathLabel) {}

void InputPath::execute(NodePorts &nodePorts) {
    const QString filePath = nodePorts.getGlobalProperty(OUTPUT_PATH);
    const std::filesystem::path path = filePath.toStdString();
    if (std::filesystem::exists(path)) {
        displayPathLabel_->setText(filePath);
        nodePorts.output<PathData>(OUTPUT_PATH, path);
    } else {
        displayPathLabel_->setText("Invalid path: " + filePath);
        nodePorts.output<PathData>(OUTPUT_PATH, "");
    }
}

CreatorWithoutParameters InputPath::creator(const QString &category) {
    return [category](
                   const std::shared_ptr<const QtNodes::ConvertersRegister> &converters_register) {
        auto *pathDisplayLabel = new QLabel();
        pathDisplayLabel->setAlignment(Qt::AlignCenter);
        pathDisplayLabel->setStyleSheet("border: 1px solid grey;");
        NitroNodeBuilder builder("File path", "FilePath", category);
        return builder.withOperator(std::make_unique<InputPath>(pathDisplayLabel))
                ->withLoadButton(OUTPUT_PATH, "Any folder or file (*.*)", OUTPUT_PATH)
                ->withIcon("image_source.png")
                ->withNodeColor(NITRO_IMAGE_COLOR)
                ->build(converters_register);
    };
}

} // namespace nitro::ImCore
