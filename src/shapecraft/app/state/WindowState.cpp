#include "WindowState.hpp"
#include "DocumentActions.hpp"
#include "File.hpp"
#include "shapecraft/document/BottoleShapeNode.hpp"
#include "shapecraft/document/BoxShapeNode.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include <QtDebug>

namespace shapecraft {

WindowState::WindowState()
    : _file(std::make_shared<File>()),
      _documentActions(std::make_shared<DocumentActions>(_file->document())) {

    auto scene = std::make_shared<Scene>(document()->history());
    document()->appendChildNode(scene);
    document()->setCurrentScene(scene);

    auto bottle = std::make_shared<BottoleShapeNode>(document()->history());
    bottle->setName(tr("Bottle"));
    scene->appendChildNode(bottle);

    auto box = std::make_shared<BoxShapeNode>(document()->history());
    box->setName(tr("Box"));
    scene->appendChildNode(box);
}

const SP<Document> &WindowState::document() const {
    return _file->document();
}

} // namespace shapecraft
