#include "HoverableRenderable.hpp"

namespace shapecraft {
namespace viewport {

void HoverableRenderable::hoverEnterEvent(const Renderable::MouseEvent &event) {
    Q_UNUSED(event)
    _isHovered = true;
    emit hoveredChanged(_isHovered);
    emit updated();
}

void HoverableRenderable::hoverLeaveEvent() {
    _isHovered = false;
    emit hoveredChanged(_isHovered);
    emit updated();
}

} // namespace viewport
} // namespace shapecraft
