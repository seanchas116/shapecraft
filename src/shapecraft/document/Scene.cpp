#include "Scene.hpp"

namespace shapecraft {

Scene::Scene(const SP<History> &history) : Node(history) {
}

bool Scene::canHaveChildren() const {
    return true;
}

bool Scene::canInsertNode(const SP<Node> &node) const {
    Q_UNUSED(node)
    // TODO
    return true;
}

SP<Node> Scene::newInstance(const SP<History> &history) const {
    return std::make_shared<Scene>(history);
}

const char *Scene::type() const {
    return "scene";
}

std::vector<SP<Node>> Scene::normalizedSelectedNodes() const {
    // * sort selected nodes
    // * do not select descendants of selected nodes

    std::vector<SP<Node>> nodes;
    forEachDescendant([&](const SP<Node> &node) {
        if (_selectedNodes.find(node) == _selectedNodes.end()) {
            return;
        }
        for (auto &&ancestor : node->ancestors()) {
            if (_selectedNodes.find(ancestor) != _selectedNodes.end()) {
                return;
            }
        }
        nodes.push_back(node);
    });
    return nodes;
}

void Scene::setSelectedNodes(const std::unordered_set<SP<Node>> &nodes) {
    if (_selectedNodes == nodes) {
        return;
    }
    _selectedNodes = nodes;
    emit selectedNodesChanged(nodes);
}

void Scene::selectNode(const SP<Node> &node, bool append) {
    std::unordered_set<SP<Node>> nodes;
    if (append) {
        nodes = _selectedNodes;
    }
    nodes.insert(node);
    setSelectedNodes(nodes);
}

void Scene::insertNodeToCurrentPosition(const SP<Node> &node) {
    // TODO: better insertion positon
    appendChildNode(node);
}

void Scene::deleteSelectedNodes() {
    auto nodes = _selectedNodes;
    for (auto &node : nodes) {
        auto maybeParent = node->parentNode();
        if (!maybeParent) {
            continue;
        }
        auto &&parent = *maybeParent;
        parent->removeChildNode(node);
    }
    setSelectedNodes({});
}

} // namespace shapecraft
