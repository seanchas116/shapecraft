#include "WindowState.hpp"
#include "DocumentActions.hpp"
#include "File.hpp"
#include "shapecraft/document/BottleShapeNode.hpp"
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

    auto bottle = std::make_shared<BottleShapeNode>(document()->history());
    bottle->setName(tr("Bottle"));
    bottle->setBoundingBox(Box<double>::fromSize(glm::dvec3(0), glm::dvec3(10, 1, 10)));
    scene->appendChildNode(bottle);

    auto box = std::make_shared<BoxShapeNode>(document()->history());
    box->setName(tr("Box"));
    box->setBoundingBox(Box<double>::fromSize(glm::dvec3(0), glm::dvec3(1, 1, 1)));
    scene->appendChildNode(box);
}

const SP<Document> &WindowState::document() const {
    return _file->document();
}

} // namespace shapecraft
