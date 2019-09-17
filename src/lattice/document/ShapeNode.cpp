#include "ShapeNode.hpp"

namespace lattice {

ShapeNode::ShapeNode(const SP<History> &history) : Node(history) {
}

bool ShapeNode::canHaveChildren() const {
    return true;
}

bool ShapeNode::canInsertNode(const SP<Node> &node) const {
    return !!std::dynamic_pointer_cast<ShapeNode>(node);
}

SP<Node> ShapeNode::newInstance(const SP<History> &history) const {
    return std::make_shared<ShapeNode>(history);
}

QString ShapeNode::type() const {
    return "shape";
}

} // namespace lattice
