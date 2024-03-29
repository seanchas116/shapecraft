#include "ShapeNode.hpp"
#include "history/PropertyChange.hpp"

namespace shapecraft {

ShapeNode::ShapeNode(const SP<History> &history) : Node(history) {
}

bool ShapeNode::canHaveChildren() const {
    return true;
}

bool ShapeNode::canInsertNode(const SP<Node> &node) const {
    return !!std::dynamic_pointer_cast<ShapeNode>(node);
}

void ShapeNode::setShape(const TopoDS_Shape &shape) {
    _shape = shape;
    emit shapeChanged(shape);
}

void ShapeNode::saveJSON(nlohmann::json &json) const {
    super::saveJSON(json);
    json["boundingBox"] = boundingBox();
}

void ShapeNode::loadJSON(const nlohmann::json &json) {
    super::loadJSON(json);
    setBoundingBox(json.at("boundingBox"));
}

} // namespace shapecraft
