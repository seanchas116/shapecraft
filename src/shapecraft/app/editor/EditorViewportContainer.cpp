#include "EditorViewportContainer.hpp"
#include "shapecraft/render/viewport/Viewport.hpp"
#include <QVBoxLayout>

namespace shapecraft {

EditorViewportContainer::EditorViewportContainer(QWidget *parent) : viewport::ViewportContainer(parent) {
    auto viewport = new viewport::Viewport();
    auto layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(viewport);
    setLayout(layout);
}

} // namespace shapecraft
