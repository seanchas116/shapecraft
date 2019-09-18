#pragma once
#include "Node.hpp"

namespace shapecraft {

class Document final : public Node {
    Q_OBJECT
  public:
    Document();

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    SP<Node> newInstance(const SP<History> &history) const override;
    QString type() const override;
};

} // namespace shapecraft
