#pragma once

#include "shapecraft/Common.hpp"
#include "shapecraft/render/draw/DrawMethods.hpp"
#include "shapecraft/util/Camera.hpp"
#include <QObject>
#include <glm/glm.hpp>

class QMouseEvent;
class QContextMenuEvent;

namespace shapecraft {

namespace draw {
class DrawMethods;
}

namespace viewport {

class Viewport;

glm::vec4 valueToColor(uint64_t value);
uint64_t colorToValue(glm::vec4 color);

class Renderable : public QObject, public std::enable_shared_from_this<Renderable> {
    Q_OBJECT
  public:
    struct DrawEvent {
        Viewport *viewport;
        Camera camera;
        SP<draw::DrawMethods> drawMethods;
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

    Renderable() {}
    virtual ~Renderable();

    bool isVisible() const { return _isVisible; }
    void setVisible(bool visible);

    auto &childRenderables() const { return _childRenderables; }
    void setChildRenderables(const std::vector<SP<Renderable>> &children);

    void preDrawRecursive(const DrawEvent &event);
    void drawRecursive(const DrawEvent &event);
    void drawHitAreaRecursive(const DrawEvent &event, std::vector<SP<Renderable>> &drawnRenderables);
    void draw2DRecursive(const Draw2DEvent &event);

    virtual void preDraw(const DrawEvent &event);
    virtual void draw(const DrawEvent &event);
    virtual void drawHitArea(const DrawEvent &event, glm::vec4 hitColor);
    virtual void draw2D(const Draw2DEvent &event);

    virtual void mousePressEvent(const MouseEvent &event);
    virtual void mouseMoveEvent(const MouseEvent &event);
    virtual void mouseReleaseEvent(const MouseEvent &event);
    virtual void mouseDoubleClickEvent(const MouseEvent &event);
    virtual void contextMenuEvent(const MouseEvent &event);
    virtual void hoverEnterEvent(const MouseEvent &event);
    virtual void hoverLeaveEvent();

  signals:
    void updated();

  private:
    std::vector<SP<Renderable>> _childRenderables;
    bool _isVisible = true;
};

} // namespace viewport
} // namespace shapecraft
