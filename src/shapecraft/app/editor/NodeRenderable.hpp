#pragma once
#include "shapecraft/render/viewport/Renderable.hpp"
#include <QObject>
#include <TopoDS_Shape.hxx>

namespace shapecraft {
class Node;
namespace gl {
class VAO;
}

class NodeRenderable final : public viewport::Renderable {
    Q_OBJECT
  public:
    NodeRenderable(const SP<Node> &node);

  private:
    void setShape(const TopoDS_Shape &shape);

  private:
    std::shared_ptr<gl::VAO> _mesh;
    std::shared_ptr<gl::VAO> _edgeMesh;
};

} // namespace shapecraft
