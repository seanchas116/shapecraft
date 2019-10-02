#include "Document.hpp"
#include "Scene.hpp"
#include "history/History.hpp"

namespace shapecraft {

Document::Document() : Node(std::make_shared<History>()) {
}

void Document::setCurrentScene(const SP<Scene> &scene) {
    if (_currentScene == scene) {
        return;
    }

    if (_currentScene) {
        disconnect(_currentScene.get(), &Scene::selectedNodesChanged, this, &Document::selectedNodesChanged);
    }

    _currentScene = scene;

    connect(scene.get(), &Scene::selectedNodesChanged, this, &Document::selectedNodesChanged);
    emit selectedNodesChanged(selectedNodes());

    emit currentSceneChanged(scene);
}

const std::unordered_set<SP<Node>> &Document::selectedNodes() const {
    return _currentScene->selectedNodes();
}

std::vector<SP<Scene>> Document::scenes() const {
    std::vector<SP<Scene>> scenes;
    for (auto &&child : childNodes()) {
        scenes.push_back(std::dynamic_pointer_cast<Scene>(child));
    }
    return scenes;
}

bool Document::canHaveChildren() const {
    return true;
}

bool Document::canInsertNode(const SP<Node> &node) const {
    return !!std::dynamic_pointer_cast<Scene>(node);
}

SP<Node> Document::newInstance(const SP<History> &history) const {
    Q_UNUSED(history)
    throw std::runtime_error("Document cannot be cloned");
}

const char *Document::type() const {
    return "document";
}

} // namespace shapecraft
