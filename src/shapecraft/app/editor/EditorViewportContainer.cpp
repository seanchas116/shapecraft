#include "EditorViewportContainer.hpp"
#include "Background.hpp"
#include "EditorViewport.hpp"
#include "GridFloor.hpp"
#include "shapecraft/util/KeyObserver.hpp"
#include <QVBoxLayout>

namespace shapecraft {

EditorViewportContainer::EditorViewportContainer(const SP<WindowState> &state, QWidget *parent)
    : viewport::ViewportContainer(parent),
      _state(state),
      _keyObserver(std::make_shared<KeyObserver>()) {

    auto viewport = new EditorViewport(state, _keyObserver);

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
