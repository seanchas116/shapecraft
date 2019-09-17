#pragma once
#include "Node.hpp"

namespace lattice {

class ShapeNode final : public Node {
    Q_OBJECT
  public:
    ShapeNode(const SP<History> &history);

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    SP<Node> newInstance(const SP<History> &history) const override;
    QString type() const override;
};

} // namespace lattice
