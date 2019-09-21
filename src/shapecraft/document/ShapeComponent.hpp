#pragma once
#include "shapecraft/Common.hpp"
#include <QObject>
#include <TopoDS_Shape.hxx>

namespace shapecraft {

class Node;
class History;

class ShapeComponent : public QObject {
    Q_OBJECT
  public:
    ShapeComponent(const SP<Node> &node, const SP<History> &history);

    virtual TopoDS_Shape shape() const = 0;

  private:
    WP<Node> _node;
    SP<History> _history;
};

} // namespace shapecraft
