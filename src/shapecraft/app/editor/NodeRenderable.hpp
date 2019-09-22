#pragma once
#include "shapecraft/render/viewport/Renderable.hpp"
#include "shapecraft/util/Location.hpp"
#include <QObject>
#include <TopoDS_Shape.hxx>

namespace shapecraft {
class Node;
class Scene;
namespace gl {
class VertexArray;
}

class NodeRenderable final : public viewport::Renderable {
    Q_OBJECT
  public:
    NodeRenderable(const SP<Scene> &scene, const SP<Node> &node);

    void draw(const viewport::DrawEvent &event) override;
    void drawHitArea(const viewport::DrawEvent &event) override;

    void mousePressEvent(const viewport::MouseEvent &event) override;
    void mouseMoveEvent(const viewport::MouseEvent &event) override;
    void mouseReleaseEvent(const viewport::MouseEvent &event) override;

  private:
    void setShape(const TopoDS_Shape &shape);

  private:
    SP<Scene> _scene;
    SP<Node> _node;

    SP<gl::VertexArray> _facesVAO;
    SP<gl::VertexArray> _edgesVAO;

    bool _dragged = false;
    Location _dragInitLocation;
    glm::dvec3 _dragInitWorldPos;
    glm::dvec2 _dragInitViewportPos;
    bool _dragStarted = false;
};

} // namespace shapecraft
