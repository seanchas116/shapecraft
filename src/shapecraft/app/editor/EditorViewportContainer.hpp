#pragma once
#include "shapecraft/render/viewport/ViewportContainer.hpp"
#include <QWidget>

namespace shapecraft {

class WindowState;

class EditorViewportContainer : public viewport::ViewportContainer {
    Q_OBJECT
  public:
    explicit EditorViewportContainer(const SP<WindowState> &state, QWidget *parent = nullptr);

  private:
    SP<WindowState> _state;
};

} // namespace shapecraft
