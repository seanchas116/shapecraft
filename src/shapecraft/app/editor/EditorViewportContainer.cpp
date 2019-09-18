#include "EditorViewportContainer.hpp"
#include "shapecraft/render/viewport/Viewport.hpp"
#include <QVBoxLayout>

namespace shapecraft {

EditorViewportContainer::EditorViewportContainer(QWidget *parent) : Viewport::ViewportContainer(parent) {
    auto viewport = new Viewport::Viewport();
    auto layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(viewport);
    setLayout(layout);
}

} // namespace shapecraft
