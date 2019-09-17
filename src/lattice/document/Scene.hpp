#pragma once
#include "Node.hpp"

namespace lattice {

class Scene : public Node {
    Q_OBJECT
  public:
    Scene(const SP<History> &history);
};

} // namespace lattice
