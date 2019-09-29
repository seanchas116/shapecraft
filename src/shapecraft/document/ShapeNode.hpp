#pragma once
#include "Node.hpp"
#include "shapecraft/util/Box.hpp"
#include "shapecraft/util/Location.hpp"
#include <TopoDS_Shape.hxx>

namespace shapecraft {

class ShapeNode : public Node {
    Q_OBJECT
  public:
    ShapeNode(const SP<History> &history);

    SHAPECRAFT_UNDOABLE_PROPERTY(Box<double>, boundingBox, setBoundingBox, {})

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;

    TopoDS_Shape shape() const { return _shape; }

  signals:
    void shapeChanged(const TopoDS_Shape &shape);

  protected:
    void setShape(const TopoDS_Shape &shape);

  private:
    TopoDS_Shape _shape;
};

} // namespace shapecraft
