#include "NodeResizeBox.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/ShapeNode.hpp"

namespace shapecraft {

NodeResizeBox::NodeResizeBox(const SP<Scene> &scene) : _scene(scene) {
    connect(_scene.get(), &Scene::selectedNodesChanged, this, &NodeResizeBox::onSelectedNodesChanged);
    connect(this, &ResizeBox::boxEdited, this, &NodeResizeBox::handleBoxEdited);
    onSelectedNodesChanged();
}

void NodeResizeBox::onSelectedNodesChanged() {
    for (auto &&node : _nodes) {
        disconnect(node.get(), &ShapeNode::boundingBoxChanged, this, &NodeResizeBox::updateBox);
    }

    auto nodes = _scene->selectedNodes();
    std::vector<SP<ShapeNode>> shapeNodes;

    for (auto &&node : nodes) {
        if (auto shapeNode = std::dynamic_pointer_cast<ShapeNode>(node)) {
            shapeNodes.push_back(shapeNode);
        }
    }

    _nodes = shapeNodes;
    setVisible(!shapeNodes.empty());

    for (auto &&node : shapeNodes) {
        connect(node.get(), &ShapeNode::boundingBoxChanged, this, &NodeResizeBox::updateBox);
    }
    updateBox();
}

void NodeResizeBox::updateBox() {
    if (_nodes.empty()) {
        return;
    }
    auto box = _nodes[0]->boundingBox();
    for (size_t i = 1; i < _nodes.size(); ++i) {
        box |= _nodes[i]->boundingBox();
    }
    setBox(box);
}

void NodeResizeBox::handleBoxEdited(const Box<double> &box) {
    Q_UNUSED(box)
}

} // namespace shapecraft
