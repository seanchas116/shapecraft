#include "BoxShapeNode.hpp"

namespace shapecraft {

BoxShapeNode::BoxShapeNode(const SP<History> &history) : ShapeNode(history) {
}

SP<Node> BoxShapeNode::newInstance(const SP<History> &history) const {
    return std::make_shared<BoxShapeNode>(history);
}

} // namespace shapecraft
