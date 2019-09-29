#pragma once
#include "Node.hpp"
#include <unordered_set>

namespace shapecraft {

class Scene final : public Node {
    Q_OBJECT
  public:
    Scene(const SP<History> &history);

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    SP<Node> newInstance(const SP<History> &history) const override;
    const char *type() const override;

    auto &&selectedNodes() const { return _selectedNodes; }
    std::vector<SP<Node>> normalizedSelectedNodes() const;
    void setSelectedNodes(const std::unordered_set<SP<Node>> &nodes);

    void selectNode(const SP<Node> &node, bool append);

  signals:
    void selectedNodesChanged(const std::unordered_set<SP<Node>> &nodes);

  private:
    std::unordered_set<SP<Node>> _selectedNodes;
};

} // namespace shapecraft
