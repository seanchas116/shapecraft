#include "WindowState.hpp"
#include "DocumentActions.hpp"
#include "File.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/TestShapeNode.hpp"
#include <QtDebug>

namespace shapecraft {

WindowState::WindowState()
    : _file(std::make_shared<File>()),
      _documentActions(std::make_shared<DocumentActions>(_file->document())) {

    auto scene = std::make_shared<Scene>(document()->history());
    document()->appendChildNode(scene);
    document()->setCurrentScene(scene);
    auto node = std::make_shared<TestShapeNode>(document()->history());
    node->setName(tr("Test"));
    scene->appendChildNode(node);
}

const SP<Document> &WindowState::document() const {
    return _file->document();
}

} // namespace shapecraft
