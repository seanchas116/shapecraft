#pragma once
#include "Node.hpp"
#include "shapecraft/util/Property.hpp"
#include <unordered_set>

namespace shapecraft {

class Scene;

class Document final : public Node {
    Q_OBJECT
  public:
    Document();

    auto &&currentScene() const { return _currentScene; }
    void setCurrentScene(const SP<Scene> &scene);

    const std::unordered_set<SP<Node>> &selectedNodes() const;

    std::vector<SP<Scene>> scenes() const; // TODO: Use range adaptor?

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    SP<Node> newInstance(const SP<History> &history) const override;
    const char *type() const override;

  signals:
    void currentSceneChanged(const SP<Scene> &scene);
    void selectedNodesChanged(const std::unordered_set<SP<Node>> &selectedNodes);

  private:
    SP<Scene> _currentScene;
};

} // namespace shapecraft
