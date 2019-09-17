#include "Scene.hpp"

namespace lattice {

Scene::Scene(const SP<History> &history) : Node(history) {
}

bool Scene::canHaveChildren() const {
    return true;
}

bool Scene::canInsertNode(const SP<Node> &node) const {
    Q_UNUSED(node)
    // TODO
    return true;
}

SP<Node> Scene::newInstance(const SP<History> &history) const {
    return std::make_shared<Scene>(history);
}

QString Scene::type() const {
    return "scene";
}

} // namespace lattice
