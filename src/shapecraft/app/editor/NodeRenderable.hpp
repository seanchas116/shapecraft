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

    void draw(const viewport::DrawEvent &event) override;

  private:
    void setShape(const TopoDS_Shape &shape);

  private:
    std::shared_ptr<gl::VAO> _facesVAO;
    std::shared_ptr<gl::VAO> _edgesVAO;
};

} // namespace shapecraft
