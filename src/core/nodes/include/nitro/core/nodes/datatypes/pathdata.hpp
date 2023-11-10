#pragma once

#include "nitro/core/nodes/datatypes/flexibledata.hpp"

#include <filesystem>

namespace nitro {

/**
 * @brief Describes an filesystem path.
 */
class PathData : public FlexibleData<std::filesystem::path, PathData> {
public:
    /**
     * @brief Creates a new empty filesystem path data type.
     */
    PathData();

    /**
     * @brief Creates a filesystem path data type with the provided value.
     * @param value The value this type should have.
     */
    explicit PathData(const std::filesystem::path &value);

    /**
     * @brief Returns the unique id of this data type.
     * @return The unique id of this data type.
     */
    static QString id() { return id_; }

    /**
     * @brief Registers the conversions that this data type allows.
     */
    static void registerConversions();

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of this data type.
     */
    [[nodiscard]] QString getDescription() const override;

private:
    inline static const QString name_ = "Path";
    inline static const QString id_ = "Path";
    inline static const QColor baseColor_ = {89, 140, 92};
};

} // namespace nitro
