#pragma once
#include "ShapeNode.hpp"

namespace shapecraft {

class CylinderShapeNode final : public ShapeNode {
    Q_OBJECT
  public:
    CylinderShapeNode(const SP<History> &history);

    const char *type() const override;
    SP<Node> newInstance(const SP<History> &history) const override;
};

} // namespace shapecraft
