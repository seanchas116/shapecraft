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

void Renderable::preDrawRecursive(const DrawEvent &event) {
    if (!_isVisible) {
        return;
    }
    preDraw(event);
    for (auto &c : childRenderables()) {
        c->preDrawRecursive(event);
    }
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

    auto index = drawnRenderables.size();
    drawnRenderables.push_back(shared_from_this());
    drawHitArea(event, valueToColor(index));

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

void Renderable::getDescendants(std::vector<SP<Renderable>> &descendants) {
    descendants.push_back(shared_from_this());
    for (auto &c : _childRenderables) {
        c->getDescendants(descendants);
    }
}

void Renderable::preDraw(const DrawEvent &event) {
    Q_UNUSED(event)
}

void Renderable::draw(const DrawEvent &event) {
    Q_UNUSED(event)
}

void Renderable::drawHitArea(const DrawEvent &event, glm::vec4 hitColor) {
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

glm::vec4 valueToColor(uint64_t value) {
    union {
        uint64_t value;
        glm::u16vec4 color;
    } idColor;
    idColor.value = value;

    return glm::vec4(idColor.color) / float(0xFFFF);
}

uint64_t colorToValue(glm::vec4 color) {
    union {
        uint64_t value;
        glm::u16vec4 color;
    } idColor;
    idColor.color = glm::u16vec4(glm::round(color * float(0xFFFF)));
    return idColor.value;
}

} // namespace viewport
} // namespace shapecraft
