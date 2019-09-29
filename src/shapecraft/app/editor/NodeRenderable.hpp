#pragma once
#include "shapecraft/render/gl/ContextRecallable.hpp"
#include "shapecraft/render/viewport/Renderable.hpp"
#include "shapecraft/util/Box.hpp"
#include "shapecraft/util/Location.hpp"
#include <QObject>
#include <TopoDS_Shape.hxx>

namespace shapecraft {
class Node;
class Scene;
namespace gl {
class VertexArray;
}

class NodeRenderable final : public viewport::Renderable, gl::ContextRecallable {
    Q_OBJECT
  public:
    NodeRenderable(const SP<Scene> &scene, const SP<Node> &node);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event) override;

    void mousePressEvent(const MouseEvent &event) override;
    void mouseMoveEvent(const MouseEvent &event) override;
    void mouseReleaseEvent(const MouseEvent &event) override;

  private:
    void setShape(const TopoDS_Shape &shape);

  private:
    void updateVAOs();

    SP<Scene> _scene;
    SP<Node> _node;

    TopoDS_Shape _shape;
    bool _isVAOsDirty = true;
    glm::dmat4 _shapeTransform;
    SP<gl::VertexArray> _facesVAO;
    SP<gl::VertexArray> _edgesVAO;

    bool _dragged = false;
    Box<double> _dragInitBoundingBox;
    glm::dvec3 _dragInitWorldPos;
    glm::dvec2 _dragInitViewportPos;
    bool _dragStarted = false;
};

} // namespace shapecraft
