#pragma once
#include "shapecraft/render/viewport/Renderable.hpp"
#include <QObject>
#include <TopoDS_Shape.hxx>

namespace shapecraft {
class Node;
namespace gl {
class VertexArray;
}

class NodeRenderable final : public viewport::Renderable {
    Q_OBJECT
  public:
    NodeRenderable(const SP<Node> &node);

    void draw(const viewport::DrawEvent &event) override;
    void drawHitArea(const viewport::DrawEvent &event) override;

    void mousePressEvent(const viewport::MouseEvent &event) override;
    void mouseMoveEvent(const viewport::MouseEvent &event) override;
    void mouseReleaseEvent(const viewport::MouseEvent &event) override;

  private:
    void setShape(const TopoDS_Shape &shape);

  private:
    SP<gl::VertexArray> _facesVAO;
    SP<gl::VertexArray> _edgesVAO;
};

} // namespace shapecraft
