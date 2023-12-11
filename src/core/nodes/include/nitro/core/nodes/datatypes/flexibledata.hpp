#pragma once

#include <QtNodes/NodeData>

namespace nitro {

/**
 * @brief A flexible data type that can contain any type.
 * @tparam T Type of the data it contains. This can be for example, int, double, cv::Mat or std::shared_ptr<cv::Mat>.
 * @tparam U Type of the derived class. Used to ensure each derived class gets its own conversion map, regardless of the type of T.
 * For example, if two data types both encapsulate an int, but do different, this type U is needed to differentiate between the two.
 */
template<class T, class U>
class FlexibleData : public QtNodes::NodeData {
public:
    /**
     * Used for nitro::NodePorts#inGetAs
     */
    using DataType = T;

    /**
     * @brief Creates a new empty flexible data type.
     */
    FlexibleData() = default;

    /**
     * @brief Creates a new instance of this flexible data type.
     * @param val The data this class should encapsulate.
     * @param id The id of this type. Should be unique among all types.
     * @param name The name of this data type.
     * @param color The color associated with this data type.
     */
    explicit FlexibleData(T val, const QString &id, const QString &name, QColor color)
        : data_(std::move(val)),
          type_({id, name, color}) {}

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of the data type. Will be displayed when the user hovers over an output port of this type.
     */
    [[nodiscard]] QString getDescription() const override { return ""; }

    /**
     * @brief Retrieves the data encapsulated by this class.
     * @return The data encapsulated by this class.
     */
    [[nodiscard]] const T &data() const noexcept { return data_; }

    /**
     * @brief Checks whether the data is empty or not.
     * @return True if the data is empty; false otherwise.
     */
    [[nodiscard]] bool empty() const override { return false; }

    /**
     * @brief Retrieves the type of this data.
     * @return The type of this data.
     */
    [[nodiscard]] QtNodes::NodeDataType type() const override { return type_; }

private:
    T data_;
    QtNodes::NodeDataType type_;
};

} // namespace nitro
