#pragma once
#include "ShapeNode.hpp"

namespace shapecraft {

class BottoleShapeNode final : public ShapeNode {
    Q_OBJECT
  public:
    BottoleShapeNode(const SP<History> &history);

    SP<Node> newInstance(const SP<History> &history) const override;
};

} // namespace shapecraft
