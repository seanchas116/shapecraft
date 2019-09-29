#pragma once
#include "ShapeNode.hpp"

namespace shapecraft {

class BoxShapeNode final : public ShapeNode {
    Q_OBJECT
  public:
    BoxShapeNode(const SP<History> &history);

    const char *type() const override;
    SP<Node> newInstance(const SP<History> &history) const override;
};

} // namespace shapecraft
