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

struct HitColor {
    HitColor(uint32_t index = std::numeric_limits<uint32_t>::max(), uint32_t innerID = 0) : index(index), innerID(innerID) {}

    HitColor withInnerID(uint32_t innerID) {
        return HitColor(index, innerID);
    }

    glm::vec4 toColor() const;
    static HitColor fromColor(glm::vec4 color);

    uint32_t index;
    uint32_t innerID;
};

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
        uint32_t innerID;
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

    void appendChildRenderable(const SP<Renderable> &renderable);

    void drawRecursive(const DrawEvent &event);
    void drawHitAreaRecursive(const DrawEvent &event, std::vector<SP<Renderable>> &drawnRenderables);
    void draw2DRecursive(const Draw2DEvent &event);

    virtual void draw(const DrawEvent &event);
    virtual void drawHitArea(const DrawEvent &event, const HitColor &hitColor);
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
