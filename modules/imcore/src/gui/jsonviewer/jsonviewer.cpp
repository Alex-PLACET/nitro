#include "jsonviewer.hpp"

#include <QApplication>
#include <QJsonValue>

namespace nitro::ImCore {

JsonViewer::JsonViewer(QWidget *parent) : QTreeView(parent), model(new QStandardItemModel(this)) {
    // const QColor backgroundCol = QApplication::palette().color(QPalette::Window);
    setModel(model);
}

void JsonViewer::updateJson(const QJsonObject &jsonObject) {
    QStandardItem *rootItem = model->invisibleRootItem();
    parseJsonObject(jsonObject, rootItem);
}

void JsonViewer::clear() {
    model->clear();
}

void JsonViewer::parseJsonObject(const QJsonObject &jsonObj, QStandardItem *parentItem) {
    for (const QString &key: jsonObj.keys()) {
        QStandardItem *item = new QStandardItem(key);
        parentItem->appendRow(item);
        if (jsonObj.value(key).isObject()) {
            parseJsonObject(jsonObj.value(key).toObject(), item);
        } else if (jsonObj.value(key).isArray()) {
            parseJsonArray(jsonObj.value(key).toArray(), item);
        } else {
            QStandardItem *valueItem = new QStandardItem(jsonObj.value(key).toVariant().toString());
            item->appendRow(valueItem);
        }
    }
}

void JsonViewer::parseJsonArray(const QJsonArray &jsonArray, QStandardItem *parentItem) {
    for (const QJsonValue &value: jsonArray) {
        if (value.isObject()) {
            parseJsonObject(value.toObject(), parentItem);
        } else if (value.isArray()) {
            parseJsonArray(value.toArray(), parentItem);
        } else {
            QStandardItem *item = new QStandardItem(value.toVariant().toString());
            parentItem->appendRow(item);
        }
    }
}
} // namespace nitro::ImCore
