#include "ShapeNode.hpp"

namespace shapecraft {

ShapeNode::ShapeNode(const SP<History> &history) : Node(history) {
}

bool ShapeNode::canHaveChildren() const {
    return true;
}

bool ShapeNode::canInsertNode(const SP<Node> &node) const {
    return !!std::dynamic_pointer_cast<ShapeNode>(node);
}

QString ShapeNode::type() const {
    return "shape";
}

} // namespace shapecraft
