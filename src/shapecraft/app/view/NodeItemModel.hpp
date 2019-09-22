#pragma once

#include "shapecraft/Common.hpp"
#include <QAbstractItemModel>
#include <QUuid>

namespace shapecraft {

class Node;

class NodeItemModel final : public QAbstractItemModel {
    Q_OBJECT
  public:
    NodeItemModel(const SP<Node> &rootNode);

    auto &&rootNode() const { return _rootNode; }

    QModelIndex indexForNode(const SP<Node> &node) const;
    SP<Node> nodeForIndex(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parentIndex) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

  private:
    void connectNode(const SP<Node> &node);
    void disconnectNode(const SP<Node> &node);

    SP<Node> _rootNode;
    QUuid _uuid;
};

} // namespace shapecraft
