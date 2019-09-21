#include "ShapeComponent.hpp"

namespace shapecraft {

ShapeComponent::ShapeComponent(const SP<Node> &node, const SP<History> &history)
    : _node(node),
      _history(history) {
}

} // namespace shapecraft
