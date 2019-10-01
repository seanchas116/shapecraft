#pragma once
#include "Renderable.hpp"

namespace shapecraft {
namespace viewport {

class HoverableRenderable : public Renderable {
    Q_OBJECT
  public:
    bool isHovered() { return _isHovered; }

    void hoverEnterEvent(const MouseEvent &event) override;
    void hoverLeaveEvent() override;

  signals:
    void hoveredChanged(bool hovered);

  private:
    bool _isHovered = false;
};

} // namespace viewport
} // namespace shapecraft
