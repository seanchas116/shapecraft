#include "ShapeNode.hpp"
#include "history/PropertyChange.hpp"

namespace shapecraft {

ShapeNode::ShapeNode(const SP<History> &history) : Node(history) {
}

void ShapeNode::setLocation(const Location &location) {
    if (_location == location) {
        return;
    }
    addChange(std::make_shared<PropertyChange<Location>>(_location, location, [self = sharedFrom(this)](auto &&location) {
        self->_location = location;
        emit self->locationChanged(location);
    }));
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
