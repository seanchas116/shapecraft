#pragma once

#include "lattice/Common.hpp"
#include "lattice/render/draw/Operations.hpp"
#include "lattice/util/Camera.hpp"
#include <QObject>
#include <glm/glm.hpp>

class QMouseEvent;
class QContextMenuEvent;

namespace lattice {

namespace draw {
class Operations;
}

namespace Viewport {

class Viewport;

struct DrawEvent {
    Viewport *viewport;
    Camera camera;
    SP<draw::Operations> operations;
};

struct Draw2DEvent {
    Viewport *viewport;
    QSize viewportSize;
    QPainter *painter;
};

struct MouseEvent {
    Viewport *viewport;
    Camera camera;
    glm::dvec3 viewportPos;
    QMouseEvent *originalMouseEvent;
    QContextMenuEvent *originalContextMenuEvent;

    glm::dvec3 worldPos() const;
};

class Renderable : public QObject, public std::enable_shared_from_this<Renderable> {
    Q_OBJECT
  public:
    Renderable() {}
    virtual ~Renderable();

    bool isVisible() const { return _isVisible; }
    void setVisible(bool visible);

    auto &childRenderables() const { return _childRenderables; }
    void setChildRenderables(const std::vector<SP<Renderable>> &children);

    void preDrawRecursive(const DrawEvent &event);
    void drawRecursive(const DrawEvent &event);
    void drawHitAreaRecursive(const DrawEvent &event);
    void draw2DRecursive(const Draw2DEvent &event);

    void getDescendants(std::vector<SP<Renderable>> &descendants);

    virtual void preDraw(const DrawEvent &event);
    virtual void draw(const DrawEvent &event);
    virtual void drawHitArea(const DrawEvent &event);
    virtual void draw2D(const Draw2DEvent &event);

    virtual void mousePressEvent(const MouseEvent &event);
    virtual void mouseMoveEvent(const MouseEvent &event);
    virtual void mouseReleaseEvent(const MouseEvent &event);
    virtual void mouseDoubleClickEvent(const MouseEvent &event);
    virtual void contextMenuEvent(const MouseEvent &event);
    virtual void hoverEnterEvent(const MouseEvent &event);
    virtual void hoverLeaveEvent();

    glm::vec4 toIDColor() const;
    static Opt<SP<Renderable>> fromIDColor(glm::vec4 color);

  signals:
    void updated();

  private:
    std::vector<SP<Renderable>> _childRenderables;
    bool _isVisible = true;
};

} // namespace Viewport
} // namespace lattice
