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
    void drawHitArea(const DrawEvent &event, glm::vec4 hitColor) override;

    void hoverEnterEvent(const MouseEvent &event) override;
    void hoverLeaveEvent() override;

  private:
    void updateVAO();

    Box<double> _box;

    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;

    bool _isHovered = false;
};

} // namespace shapecraft
