#pragma once
#include "ShapeNode.hpp"

namespace shapecraft {

class TestShapeNode final : public ShapeNode {
    Q_OBJECT
  public:
    TestShapeNode(const SP<History> &history);

    SP<Node> newInstance(const SP<History> &history) const override;
    TopoDS_Shape shape() const override;

  private:
    TopoDS_Shape _shape;
};

} // namespace shapecraft
