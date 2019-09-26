#include "NodeItemModel.hpp"
#include "shapecraft/document/Node.hpp"
#include "shapecraft/document/history/History.hpp"
#include <QDataStream>
#include <QMimeData>
#include <QtDebug>

namespace shapecraft {

NodeItemModel::NodeItemModel(const SP<Node> &rootNode)
    : QAbstractItemModel(),
      _rootNode(rootNode),
      _uuid(QUuid::createUuid()) {
    connectNode(rootNode);
}

QVariant NodeItemModel::data(const QModelIndex &index, int role) const {
    auto node = nodeForIndex(index);
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return node->name();
    default:
        return QVariant();
    }
}

bool NodeItemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    auto node = nodeForIndex(index);
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        _rootNode->history()->beginChange(tr("Set Node Name"));
        node->setName(value.toString());
        return true;
    default:
        return false;
    }
}

Qt::ItemFlags NodeItemModel::flags(const QModelIndex &index) const {
    auto node = nodeForIndex(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
    if (node->canHaveChildren()) {
        flags |= Qt::ItemIsDropEnabled;
    }

    return flags;
}

QModelIndex NodeItemModel::index(int row, int column, const QModelIndex &parentIndex) const {
    Q_UNUSED(column)
    auto parent = nodeForIndex(parentIndex);
    if (int(parent->childNodes().size()) <= row) {
        return QModelIndex();
    }
    return indexForNode(parent->childNodes()[row]);
}

QModelIndex NodeItemModel::parent(const QModelIndex &child) const {
    auto node = nodeForIndex(child);
    auto maybeParent = node->parentNode();
    if (!maybeParent) {
        return QModelIndex();
    }
    return indexForNode(*maybeParent);
}

int NodeItemModel::rowCount(const QModelIndex &parent) const {
    return int(nodeForIndex(parent)->childNodes().size());
}

int NodeItemModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

Qt::DropActions NodeItemModel::supportedDropActions() const {
    return Qt::MoveAction | Qt::CopyAction;
}

namespace {
const QString _indexMimeType = "application/x-grid-node-index";
}

QStringList NodeItemModel::mimeTypes() const {
    return {_indexMimeType};
}

QMimeData *NodeItemModel::mimeData(const QModelIndexList &indexes) const {
    auto mimeData = new QMimeData();

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << _uuid;

    int rootIndexPathCount = _rootNode->indexPath().size();

    for (const auto &index : indexes) {
        auto node = nodeForIndex(index);
        stream << QVector<int>::fromStdVector(node->indexPath()).mid(rootIndexPathCount);
    }
    mimeData->setData(_indexMimeType, data);

    return mimeData;
}

bool NodeItemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    Q_UNUSED(column)

    if (action == Qt::IgnoreAction) {
        return true;
    }

    auto parentNode = nodeForIndex(parent);

    if (data->hasFormat(_indexMimeType)) {
        QByteArray encodedData = data->data(_indexMimeType);
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QUuid uuid;
        stream >> uuid;
        if (uuid != _uuid) {
            return false;
        }

        std::vector<SP<Node>> nodes;

        while (!stream.atEnd()) {
            QVector<int> indexPath;
            stream >> indexPath;

            SP<Node> node = _rootNode;
            for (int index : indexPath) {
                node = node->childNodes()[index];
            }

            nodes.push_back(node);
        }

        Opt<SP<Node>> ref;
        if (row != -1 && row != int(parentNode->childNodes().size())) {
            ref = parentNode->childNodes()[row];
        }
        switch (action) {
        default:
        case Qt::CopyAction:
            _rootNode->history()->beginChange(tr("Copy Nodes"));
            for (const auto &node : nodes) {
                parentNode->insertNodeBefore(node->clone(_rootNode->history()), ref); // TODO: clone recursively
            }
            break;
        case Qt::MoveAction:
            _rootNode->history()->beginChange(tr("Move Nodes"));
            for (const auto &node : nodes) {
                parentNode->insertNodeBefore(node, ref);
            }
            break;
        }

        return true;
    }
    return false;
}

QModelIndex NodeItemModel::indexForNode(const SP<Node> &node) const {
    if (node == _rootNode) {
        return {};
    }
    return createIndex(node->index(), 0, node.get());
}

SP<Node> NodeItemModel::nodeForIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return _rootNode;
    }
    return static_cast<Node *>(index.internalPointer())->shared_from_this();
}

void NodeItemModel::connectNode(const SP<Node> &node) {
    auto nodePtr = node.get();
    connect(nodePtr, &Node::nameChanged, this, [this, nodePtr]() {
        auto index = indexForNode(nodePtr->shared_from_this());
        emit dataChanged(index, index, {Qt::DisplayRole});
    });
    connect(nodePtr, &Node::childNodesAboutToBeInserted, this, [this, nodePtr](int first, int last) {
        auto index = indexForNode(nodePtr->shared_from_this());
        beginInsertRows(index, first, last);
    });
    connect(nodePtr, &Node::childNodesInserted, this, [this, nodePtr](int first, int last) {
        for (int i = first; i <= last; ++i) {
            auto &child = nodePtr->childNodes()[i];
            connectNode(child);
        }
        endInsertRows();
    });
    connect(nodePtr, &Node::childNodesAboutToBeRemoved, this, [this, nodePtr](int first, int last) {
        for (int i = first; i <= last; ++i) {
            auto &child = nodePtr->childNodes()[i];
            disconnectNode(child);
        }
        auto index = indexForNode(nodePtr->shared_from_this());
        beginRemoveRows(index, first, last);
    });
    connect(nodePtr, &Node::childNodesRemoved, this, [this]() {
        endRemoveRows();
    });

    for (auto &child : node->childNodes()) {
        connectNode(child);
    }
}

void NodeItemModel::disconnectNode(const SP<Node> &node) {
    disconnect(node.get(), nullptr, this, nullptr);
}

} // namespace shapecraft
