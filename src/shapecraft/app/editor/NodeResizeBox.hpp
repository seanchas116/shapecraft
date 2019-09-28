#pragma once
#include "ResizeBox.hpp"

namespace shapecraft {

class Scene;

class NodeResizeBox : public ResizeBox {
    Q_OBJECT
  public:
    NodeResizeBox(const SP<Scene> &scene);

  private:
    SP<Scene> _scene;
};

} // namespace shapecraft
