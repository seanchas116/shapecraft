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

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    QString type() const override;

    virtual Box<double> boundingBox() const = 0;
    virtual TopoDS_Shape shape() const = 0;
};

} // namespace shapecraft
