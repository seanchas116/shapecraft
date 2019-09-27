#pragma once
#include "Node.hpp"
#include "shapecraft/util/Property.hpp"

namespace shapecraft {

class Scene;

class Document final : public Node {
    Q_OBJECT
  public:
    Document();

    std::vector<SP<Scene>> scenes() const; // TODO: Use range adaptor?

    bool canHaveChildren() const override;
    bool canInsertNode(const SP<Node> &node) const override;
    SP<Node> newInstance(const SP<History> &history) const override;
    QString type() const override;

  private:
    SHAPECRAFT_PROPERTY(SP<Scene>, currentScene, setCurrentScene, {})
};

} // namespace shapecraft
