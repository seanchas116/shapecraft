#include "EditorViewportContainer.hpp"
#include "Background.hpp"
#include "EditorViewport.hpp"
#include "GridFloor.hpp"
#include "NodeRenderable.hpp"
#include "NodeResizeBox.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/ShapeNode.hpp"
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

        auto scene = _state->document()->scenes()[0];
        auto node = scene->childNodes()[0];
        auto shapeNode = std::dynamic_pointer_cast<ShapeNode>(node);

        auto rootRenderable = std::make_shared<NodeRenderable>(scene, scene);

        auto resizeBox = std::make_shared<NodeResizeBox>(scene);

        root->setChildRenderables({background, gridFloor, rootRenderable});
        setLayers({root, resizeBox});
    });
}

} // namespace shapecraft
