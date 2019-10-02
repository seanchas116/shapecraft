#include "Node.hpp"
#include "history/Change.hpp"
#include "history/History.hpp"
#include "history/PropertyChange.hpp"
#include "shapecraft/util/JSON.hpp"
#include <QMimeData>
#include <QRectF>
#include <QtDebug>

namespace shapecraft {

class Node::ChildInsertChange : public Change {
  public:
    ChildInsertChange(const SP<Node> &parent, const SP<Node> &node, const Opt<SP<const Node>> &reference)
        : _parent(parent),
          _node(node),
          _reference(reference) {
    }

    void apply() override {
        _parent->insertNodeBeforeInternal(_node, _reference);
    }

    SP<Change> invert() const override;

  private:
    SP<Node> _parent;
    SP<Node> _node;
    Opt<SP<const Node>> _reference;
};

class Node::ChildRemoveChange : public Change {
  public:
    ChildRemoveChange(const SP<Node> &parent, const SP<Node> &node)
        : _parent(parent),
          _node(node) {
    }

    void apply() override {
        _reference = _node->nextNode();
        _parent->removeChildNodeInternal(_node);
    }

    SP<Change> invert() const override;

  private:
    SP<Node> _parent;
    SP<Node> _node;
    Opt<SP<Node>> _reference;
};

SP<Change> Node::ChildInsertChange::invert() const {
    return std::make_shared<ChildRemoveChange>(_parent, _node);
}

SP<Change> Node::ChildRemoveChange::invert() const {
    return std::make_shared<ChildInsertChange>(_parent, _node, _reference);
}

Node::Node(const SP<History> &history) : _history(history) {
}

Opt<SP<Node>> Node::parentNode() const {
    auto p = _parentNode.lock();
    if (!p) {
        return std::nullopt;
    }
    return p;
}

Opt<SP<Node>> Node::nextNode() const {
    auto parent = _parentNode.lock();
    if (!parent) {
        return std::nullopt;
    }

    auto it = std::find(parent->_childNodes.begin(), parent->_childNodes.end(), shared_from_this());
    if (it == parent->_childNodes.end() || it == parent->_childNodes.end() - 1) {
        return {};
    }
    return *(it + 1);
}

void Node::setChildNodes(const std::vector<SP<Node>> &nodes) {
    if (_childNodes == nodes) {
        return;
    }
    // TODO: be more efficient
    auto children = _childNodes;
    for (auto &&child : children) {
        removeChildNode(child);
    }
    for (auto &&node : nodes) {
        insertNodeBefore(node, {});
    }
}

void Node::appendChildNode(const SP<Node> &node) {
    insertNodeBefore(node, {});
}

void Node::insertNodeBefore(const SP<Node> &node, const Opt<SP<const Node>> &reference) {
    if (node == reference) {
        return;
    }

    if (auto maybeOldParent = node->parentNode(); maybeOldParent) {
        (*maybeOldParent)->removeChildNode(node);
    }

    addChange(std::make_shared<ChildInsertChange>(shared_from_this(), node, reference));
}

void Node::removeChildNode(const SP<Node> &node) {
    addChange(std::make_shared<ChildRemoveChange>(shared_from_this(), node));
}

int Node::index() const {
    auto parent = _parentNode.lock();
    if (!parent) {
        return -1;
    }

    auto &siblings = parent->_childNodes;
    auto it = std::find(siblings.begin(), siblings.end(), shared_from_this());
    if (it == siblings.end()) {
        return -1;
    }
    return it - siblings.begin();
}

std::vector<int> Node::indexPath() const {
    auto parent = _parentNode.lock();
    if (!parent) {
        return {};
    }

    auto path = parent->indexPath();
    path.push_back(index());
    return path;
}

std::vector<SP<Node>> Node::ancestors() const {
    auto parent = _parentNode.lock();
    if (!parent) {
        return {};
    }

    auto ancestors = parent->ancestors();
    ancestors.push_back(parent);
    return ancestors;
}

void Node::forEachDescendant(const std::function<void(const SP<Node> &)> &callback) const {
    for (auto &child : _childNodes) {
        callback(child);
        child->forEachDescendant(callback);
    }
}

SP<Node> Node::clone(const SP<History> &history) const {
    auto other = newInstance(history);
    nlohmann::json json;
    saveJSON(json);
    other->loadJSON(json);
    return other;
}

void Node::saveJSON(nlohmann::json &json) const {
    json["type"] = type();
    json["name"] = _name;
}

void Node::loadJSON(const nlohmann::json &json) {
    setName(json.at("name"));
}

nlohmann::json Node::toJSONRecursive() const {
    nlohmann::json json;
    saveJSON(json);

    std::vector<nlohmann::json> childJSONs;
    for (auto &&child : _childNodes) {
        childJSONs.push_back(child->toJSONRecursive());
    }
    json["children"] = childJSONs;

    return json;
}

SP<Node> Node::fromJSONRecursive(const nlohmann::json &json, const SP<History> &history) {
    QString type = json.at("type");
    auto prototype = _prototypes.at(type);

    auto node = prototype->clone(history);
    node->loadJSON(json);

    std::vector<nlohmann::json> childJSONs = json.at("children");
    for (auto &&childJSON : childJSONs) {
        node->appendChildNode(Node::fromJSONRecursive(childJSON, history));
    }
    return node;
}

void Node::addPrototype(const SP<Node> &prototype) {
    _prototypes.insert({prototype->type(), prototype});
}

std::unordered_map<QString, SP<Node>> Node::_prototypes;

QString Node::nodesMimeType() {
    return "application/x-shapecraft-nodes";
}

QMimeData *Node::toNodesMimeData(const std::vector<SP<Node>> &nodes) {
    auto mimeData = new QMimeData();
    nlohmann::json nodesData;
    for (auto &&node : nodes) {
        nodesData.push_back(node->toJSONRecursive());
    }

    mimeData->setData(nodesMimeType(), QByteArray::fromStdString(nodesData.dump()));
    return mimeData;
}

std::vector<SP<Node>> Node::fromNodesMimeData(const QMimeData *mimeData, const SP<History> &history) {
    if (!mimeData->hasFormat(Node::nodesMimeType())) {
        return {};
    }

    auto nodesDataString = mimeData->data(nodesMimeType()).toStdString();
    std::vector<nlohmann::json> nodesData = nlohmann::json::parse(nodesDataString);
    std::vector<SP<Node>> nodes;
    for (auto &&nodeData : nodesData) {
        auto node = Node::fromJSONRecursive(nodeData, history);
        nodes.push_back(node);
    }
    return nodes;
}

void Node::addChange(const SP<Change> &change) {
    _history->addChange(change);
}

void Node::insertNodeBeforeInternal(const SP<Node> &node, const Opt<SP<const Node>> &reference) {
    if (!canHaveChildren() || !canInsertNode(node)) {
        throw std::runtime_error("cannot insert node");
    }
    if (node->parentNode()) {
        throw std::runtime_error("node already has parent");
    }

    decltype(_childNodes)::iterator it;
    if (reference) {
        it = std::find(_childNodes.begin(), _childNodes.end(), *reference);
        if (it == _childNodes.end()) {
            throw std::runtime_error("cannot find reference node");
        }
    } else {
        it = _childNodes.end();
    }
    int index = it - _childNodes.begin();
    emit childNodesAboutToBeInserted(index, index, {node});
    _childNodes.insert(it, node);
    node->_parentNode = shared_from_this();
    emit childNodesInserted(index, index);
}

void Node::removeChildNodeInternal(const SP<Node> &node) {
    auto it = std::find(_childNodes.begin(), _childNodes.end(), node);
    if (it == _childNodes.end()) {
        throw std::runtime_error("cannot find node");
    }
    int index = it - _childNodes.begin();
    emit childNodesAboutToBeRemoved(index, index);
    _childNodes.erase(it);
    node->_parentNode.reset();
    emit childNodesRemoved(index, index, {node});
}

} // namespace shapecraft
