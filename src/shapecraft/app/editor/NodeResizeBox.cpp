#include "NodeResizeBox.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/ShapeNode.hpp"

namespace shapecraft {

NodeResizeBox::NodeResizeBox(const SP<Scene> &scene) : _scene(scene) {
    updateBox();
    connect(_scene.get(), &Scene::selectedNodesChanged, this, &NodeResizeBox::updateBox);
}

void NodeResizeBox::updateBox() {
    auto shapeNodes = selectedShapeNodes();

    setVisible(!shapeNodes.empty());

    if (!shapeNodes.empty()) {
        auto box = shapeNodes[0]->boundingBox();
        for (size_t i = 1; i < shapeNodes.size(); ++i) {
            box |= shapeNodes[i]->boundingBox();
        }
        setBox(box);
    }
}

std::vector<SP<ShapeNode>> NodeResizeBox::selectedShapeNodes() const {
    auto nodes = _scene->selectedNodes();
    std::vector<SP<ShapeNode>> shapeNodes;

    for (auto &&node : nodes) {
        if (auto shapeNode = std::dynamic_pointer_cast<ShapeNode>(node)) {
            shapeNodes.push_back(shapeNode);
        }
    }
    return shapeNodes;
}

} // namespace shapecraft
