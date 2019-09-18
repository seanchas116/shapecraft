#pragma once
#include "shapecraft/render/viewport/Renderable.hpp"

namespace shapecraft {

class WindowState;

class Background : public viewport::Renderable, protected QOpenGLExtraFunctions {
    Q_OBJECT
  public:
    Background(const SP<WindowState> &appState);
    void draw(const viewport::DrawEvent &event) override;
    void drawHitArea(const viewport::DrawEvent &event) override;
    void mousePressEvent(const viewport::MouseEvent &event) override;

  private:
    SP<WindowState> _appState;
};

} // namespace shapecraft
