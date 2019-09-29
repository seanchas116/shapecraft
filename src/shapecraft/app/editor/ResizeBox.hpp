#pragma once

#include "shapecraft/render/viewport/Renderable.hpp"
#include "shapecraft/util/Box.hpp"
#include <QObject>

namespace shapecraft {

class ResizeBox : public viewport::Renderable {
    Q_OBJECT
  public:
    ResizeBox();

    void setBox(const Box<double> &box);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void hoverEnterEvent(const MouseEvent &event) override;
    void hoverLeaveEvent() override;

  private:
    void updateVAOs();

    Box<double> _box;

    bool _isVAOsDirty = true;
    SP<gl::VertexArray> _edgesVAO;

    bool _isHovered = false;
};

} // namespace shapecraft
