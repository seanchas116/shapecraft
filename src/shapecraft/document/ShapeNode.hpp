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

    void setLocation(const Location &location);
    auto &&location() const { return _location; }

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    QString type() const override;

    virtual Box<double> boundingBox() const = 0;
    virtual TopoDS_Shape shape() const = 0;

  signals:
    void locationChanged(const Location &location);

  private:
    Location _location;
};

} // namespace shapecraft
