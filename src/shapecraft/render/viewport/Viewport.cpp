#include "Viewport.hpp"
#include "HitAreaMap.hpp"
#include "Renderable.hpp"
#include "Util.hpp"
#include "shapecraft/util/Debug.hpp"
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QResizeEvent>

namespace shapecraft {
namespace viewport {

Viewport::Viewport(QWidget *parent) : QWidget(parent), _camera(Camera::perspective(glm::dmat4(1), glm::dvec2(1))) {
    setMouseTracking(true);
}

void Viewport::setCamera(const Camera &camera) {
    _camera = camera;
    emit updateRequested();
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    auto pos = mapQtToGL(this, event->pos());

    auto maybeHitResult = hitTest(pos, _camera);
    if (!maybeHitResult) {
        return;
    }

    auto hitResult = *maybeHitResult;

    Renderable::MouseEvent renderMouseEvent{this, _camera, glm::dvec3(pos, hitResult.depth), hitResult.customValue, event, nullptr};
    hitResult.renderable->mousePressEvent(renderMouseEvent);
    _draggedHitResult = hitResult;
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    auto pos = mapQtToGL(this, event->pos());

    if (_draggedHitResult) {
        // drag
        auto hitResult = *_draggedHitResult;
        Renderable::MouseEvent renderMouseEvent{this, _camera, glm::dvec3(pos, hitResult.depth), hitResult.customValue, event, nullptr};
        hitResult.renderable->mouseMoveEvent(renderMouseEvent);
        return;
    } else {
        // hover
        auto maybeHitResult = hitTest(pos, _camera);
        if (!maybeHitResult) {
            if (_hoveredHitResult) {
                (*_hoveredHitResult).renderable->hoverLeaveEvent();
            }
            return;
        }
        auto hitResult = *maybeHitResult;
        Renderable::MouseEvent renderMouseEvent{this, _camera, glm::dvec3(pos, hitResult.depth), hitResult.customValue, event, nullptr};
        if (_hoveredHitResult && _hoveredHitResult->renderable == hitResult.renderable) {
            hitResult.renderable->mouseMoveEvent(renderMouseEvent);
        } else {
            if (_hoveredHitResult) {
                _hoveredHitResult->renderable->hoverLeaveEvent();
            }
            hitResult.renderable->hoverEnterEvent(renderMouseEvent);
            hitResult.renderable->mouseMoveEvent(renderMouseEvent);
            _hoveredHitResult = hitResult;
        }
    }
}

void Viewport::mouseDoubleClickEvent(QMouseEvent *event) {
    auto pos = mapQtToGL(this, event->pos());

    auto maybeHitResult = hitTest(pos, _camera);
    if (!maybeHitResult) {
        return;
    }

    auto [renderable, customValue, hitDepth] = *maybeHitResult;

    Renderable::MouseEvent renderMouseEvent{this, _camera, glm::dvec3(pos, hitDepth), customValue, event, nullptr};
    renderable->mouseDoubleClickEvent(renderMouseEvent);
}

void Viewport::contextMenuEvent(QContextMenuEvent *event) {
    auto pos = mapQtToGL(this, event->pos());

    auto maybeHitResult = hitTest(pos, _camera);
    if (!maybeHitResult) {
        return;
    }

    auto [renderable, customValue, hitDepth] = *maybeHitResult;

    Renderable::MouseEvent renderContextMenuEvent{this, _camera, glm::dvec3(pos, hitDepth), customValue, nullptr, event};
    renderable->contextMenuEvent(renderContextMenuEvent);
}

void Viewport::moveEvent(QMoveEvent *event) {
    super::moveEvent(event);
    emit updateRequested();
}

void Viewport::resizeEvent(QResizeEvent *event) {
    super::resizeEvent(event);
    emit updateRequested();
}

const std::vector<SP<HitAreaMap>> &Viewport::hitAreaMaps(size_t count) {
    if (_hitAreaMaps.size() == count) {
        return _hitAreaMaps;
    }
    _hitAreaMaps.clear();
    for (size_t i = 0; i < count; ++i) {
        _hitAreaMaps.push_back(std::make_shared<HitAreaMap>());
    }
    return _hitAreaMaps;
}

void Viewport::mouseReleaseEvent(QMouseEvent *event) {
    if (!_draggedHitResult) {
        return;
    }
    auto hitResult = *_draggedHitResult;

    Renderable::MouseEvent renderMouseEvent{this, _camera, glm::dvec3(mapQtToGL(this, event->pos()), hitResult.depth), hitResult.customValue, event, nullptr};
    hitResult.renderable->mouseReleaseEvent(renderMouseEvent);
    _draggedHitResult = {};
}

Opt<HitResult> Viewport::hitTest(glm::dvec2 pos, const Camera &camera) {
    Q_UNUSED(camera);

    for (auto it = _hitAreaMaps.rbegin(); it != _hitAreaMaps.rend(); ++it) {
        auto result = (*it)->pick(pos);
        if (result) {
            return result;
        }
    }
    return {};
}

} // namespace viewport
} // namespace shapecraft
