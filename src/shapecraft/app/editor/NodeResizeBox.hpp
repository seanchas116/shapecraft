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
    void onSelectedNodesChanged();
    void updateBox();
    void handleBoxEdited(const Box<double> &box);

    SP<Scene> _scene;
    std::vector<SP<ShapeNode>> _nodes;
};

} // namespace shapecraft
