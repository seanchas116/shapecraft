#pragma once
#include "Node.hpp"

namespace lattice {

class ShapeNode : public Node {
    Q_OBJECT
  public:
    ShapeNode(const SP<History> &history);
};

} // namespace lattice
