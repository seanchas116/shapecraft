#pragma once
#include "HitResult.hpp"
#include "shapecraft/Common.hpp"
#include "shapecraft/render/draw/DrawMethods.hpp"
#include "shapecraft/util/Camera.hpp"
#include <QOpenGLWidget>

namespace shapecraft {
namespace viewport {

class HitAreaMap;

class Viewport : public QWidget {
    Q_OBJECT
    using super = QWidget;

  public:
    Viewport(QWidget *parent = nullptr);

    auto &camera() const { return _camera; }
    void setCamera(const Camera &camera);

  signals:
    void updateRequested();

  protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

  private:
    friend class ViewportContainer;

    const std::vector<SP<HitAreaMap>> &hitAreaMaps(size_t count);

    Opt<HitResult> hitTest(glm::dvec2 pos, const Camera &camera);

    Opt<HitResult> _draggedHitResult;
    Opt<HitResult> _hoveredHitResult;
    std::vector<SP<HitAreaMap>> _hitAreaMaps;
    Camera _camera;
};

} // namespace viewport
} // namespace shapecraft
