#pragma once
#include "history/History.hpp"
#include "node/Node.hpp"

namespace lattice {

class Document : public Node {
    Q_OBJECT
  public:
    Document();
};

} // namespace lattice
