#pragma once
#include "shapecraft/render/viewport/ViewportContainer.hpp"
#include <QWidget>

namespace shapecraft {

class WindowState;
class KeyObserver;
class NodeResizeBox;

class EditorViewportContainer : public viewport::ViewportContainer {
    Q_OBJECT
  public:
    explicit EditorViewportContainer(const SP<WindowState> &state, QWidget *parent = nullptr);

  private:
    SP<WindowState> _state;
    SP<NodeResizeBox> _resizeBox; // keep reference
};

} // namespace shapecraft
