#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>

namespace nitro::ImCore {

class JsonViewer : public QTreeView {

public:
    explicit JsonViewer(QWidget *parent = nullptr);

    void updateJson(const QJsonObject &json);

    void clear();

private:
    QStandardItemModel *model;
    void parseJsonObject(const QJsonObject &jsonObj, QStandardItem *parentItem);
    void parseJsonArray(const QJsonArray &jsonArray, QStandardItem *parentItem);
};
} // namespace nitro::ImCore
