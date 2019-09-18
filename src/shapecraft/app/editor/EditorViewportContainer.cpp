#include "EditorViewportContainer.hpp"
#include "Background.hpp"
#include "GridFloor.hpp"
#include "shapecraft/render/viewport/Viewport.hpp"
#include <QVBoxLayout>

namespace shapecraft {

EditorViewportContainer::EditorViewportContainer(const SP<WindowState> &state, QWidget *parent)
    : viewport::ViewportContainer(parent),
      _state(state) {
    auto viewport = new viewport::Viewport();

    auto layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(viewport);
    setLayout(layout);

    connect(this, &viewport::ViewportContainer::initialized, this, [=] {
        auto root = std::make_shared<viewport::Renderable>();
        auto background = std::make_shared<Background>(_state);
        auto gridFloor = std::make_shared<GridFloor>();
        root->setChildRenderables({background, gridFloor});
        viewport->setRenderable(root);
    });
}

} // namespace shapecraft
