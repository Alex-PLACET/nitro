#pragma once

#include "nitro/core/nodes/datatypes/flexibledata.hpp"

#include <QJsonObject>
#include <qjsonobject.h>

namespace nitro {

/**
 * @brief Describes a JSON data type.
 */
class JsonData : public FlexibleData<QJsonObject, JsonData> {
public:
    /**
     * @brief Creates a new empty JSON data type.
     */
    JsonData();

    /**
     * @brief Creates a JSON data type with the provided value.
     * @param value The value this type should have.
     */
    explicit JsonData(const QJsonObject &json);

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
    inline static const QString name_ = "Json";
    inline static const QString id_ = "Json";
    inline static const QColor baseColor_ = {89, 66, 92};
};

} // namespace nitro
