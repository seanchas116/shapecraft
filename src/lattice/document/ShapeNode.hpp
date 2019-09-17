#pragma once
#include "Node.hpp"
#include <TopoDS_Shape.hxx>

namespace lattice {

class ShapeNode : public Node {
    Q_OBJECT
  public:
    ShapeNode(const SP<History> &history);

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    QString type() const override;

    virtual TopoDS_Shape shape() const = 0;
};

} // namespace lattice
