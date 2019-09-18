#include "Background.hpp"
#include <QtDebug>

namespace shapecraft {

Background::Background(const SP<WindowState> &appState) : _appState(appState) {
    initializeOpenGLFunctions();
}

void Background::draw(const viewport::DrawEvent &event) {
    // TODO: manage depth test in better way
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    event.operations->clear.clear(glm::vec4(0.8, 0.8, 0.8, 1), 1);
}

void Background::drawHitArea(const viewport::DrawEvent &event) {
    auto color = toIDColor();
    event.operations->clear.clear(color, 1);
}

void Background::mousePressEvent(const viewport::MouseEvent &event) {
    Q_UNUSED(event)
    //_appState->deselectAll();
}

} // namespace shapecraft
