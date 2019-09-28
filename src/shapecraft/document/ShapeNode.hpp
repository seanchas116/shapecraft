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

    SHAPECRAFT_UNDOABLE_PROPERTY(Location, location, setLocation, {})
    SHAPECRAFT_UNDOABLE_PROPERTY(Box<double>, boundingBox, setBoundingBox, {})

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    QString type() const override;

    virtual TopoDS_Shape shape() const = 0;

  signals:
    void shapeChanged(const TopoDS_Shape &shape);
};

} // namespace shapecraft
