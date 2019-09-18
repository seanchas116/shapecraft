#include "EditorViewportContainer.hpp"
#include "Background.hpp"
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
        auto background = std::make_shared<Background>(_state);
        viewport->setRenderable(background);
    });
}

} // namespace shapecraft
