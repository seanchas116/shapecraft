#pragma once
#include "ResizeBox.hpp"

namespace shapecraft {

class Scene;
class ShapeNode;

class NodeResizeBox : public ResizeBox {
    Q_OBJECT
  public:
    NodeResizeBox(const SP<Scene> &scene);

  private:
    void updateBox();
    std::vector<SP<ShapeNode>> selectedShapeNodes() const;

    SP<Scene> _scene;
};

} // namespace shapecraft
