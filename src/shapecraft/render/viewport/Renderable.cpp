#include "Renderable.hpp"

namespace shapecraft {
namespace viewport {

glm::dvec3 Renderable::MouseEvent::worldPos() const {
    return camera.mapViewportToWorld(viewportPos);
}

Renderable::~Renderable() {
}

void Renderable::setVisible(bool visible) {
    if (_isVisible == visible) {
        return;
    }
    _isVisible = visible;
    emit updated();
}

void Renderable::setChildRenderables(const std::vector<SP<Renderable>> &children) {
    for (auto &child : _childRenderables) {
        disconnect(child.get(), &Renderable::updated, this, &Renderable::updated);
    }
    _childRenderables = children;
    for (auto &child : _childRenderables) {
        connect(child.get(), &Renderable::updated, this, &Renderable::updated);
    }
    emit updated();
}

void Renderable::appendChildRenderable(const SP<Renderable> &renderable) {
    _childRenderables.push_back(renderable);
    connect(renderable.get(), &Renderable::updated, this, &Renderable::updated);
    emit updated();
}

void Renderable::drawRecursive(const DrawEvent &event) {
    if (!_isVisible) {
        return;
    }
    draw(event);
    for (auto &c : childRenderables()) {
        c->drawRecursive(event);
    }
}

void Renderable::drawHitAreaRecursive(const DrawEvent &event, std::vector<SP<Renderable>> &drawnRenderables) {
    if (!_isVisible) {
        return;
    }

    auto index = int32_t(drawnRenderables.size());
    drawnRenderables.push_back(shared_from_this());
    drawHitArea(event, HitColor(index));

    for (auto &c : childRenderables()) {
        c->drawHitAreaRecursive(event, drawnRenderables);
    }
}

void Renderable::draw2DRecursive(const Draw2DEvent &event) {
    if (!_isVisible) {
        return;
    }
    draw2D(event);
    for (auto &c : childRenderables()) {
        c->draw2DRecursive(event);
    }
}

void Renderable::draw(const DrawEvent &event) {
    Q_UNUSED(event)
}

void Renderable::drawHitArea(const DrawEvent &event, const HitColor &hitColor) {
    Q_UNUSED(event)
    Q_UNUSED(hitColor)
}

void Renderable::draw2D(const Draw2DEvent &event) {
    Q_UNUSED(event)
}

void Renderable::mousePressEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::mouseMoveEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::mouseReleaseEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::mouseDoubleClickEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::contextMenuEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::hoverEnterEvent(const MouseEvent &event) {
    Q_UNUSED(event)
}

void Renderable::hoverLeaveEvent() {
}

namespace {

union HitColorData {
    struct {
        uint32_t index;
        uint32_t customValue;
    };
    uint64_t value;
    glm::u16vec4 color;
};

} // namespace

glm::vec4 HitColor::toColor() const {
    HitColorData data;
    data.index = index;
    data.customValue = innerID;
    return glm::vec4(data.color) / float(0xFFFF);
}

HitColor HitColor::fromColor(glm::vec4 color) {
    HitColorData data;
    data.color = glm::u16vec4(glm::round(color * float(0xFFFF)));

    return HitColor(data.index, data.customValue);
}

} // namespace viewport
} // namespace shapecraft
