#include "EditorViewportContainer.hpp"
#include "Background.hpp"
#include "EditorViewport.hpp"
#include "GridFloor.hpp"
#include "NodeRenderable.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/TestShapeNode.hpp"
#include "shapecraft/util/KeyObserver.hpp"
#include <QVBoxLayout>

namespace shapecraft {

EditorViewportContainer::EditorViewportContainer(const SP<WindowState> &state, QWidget *parent)
    : viewport::ViewportContainer(parent),
      _state(state) {

    auto viewport = new EditorViewport(state);

    auto layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(viewport);
    setLayout(layout);

    connect(this, &viewport::ViewportContainer::initialized, this, [=] {
        auto root = std::make_shared<viewport::Renderable>();
        auto background = std::make_shared<Background>(_state);
        auto gridFloor = std::make_shared<GridFloor>();
        auto nodeRenderable = std::make_shared<NodeRenderable>(std::make_shared<TestShapeNode>(state->document()->history()));
        root->setChildRenderables({background, gridFloor, nodeRenderable});
        viewport->setRenderable(root);
    });
}

} // namespace shapecraft
