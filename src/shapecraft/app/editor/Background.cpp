#include "Background.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include <QtDebug>

namespace shapecraft {

Background::Background(const SP<WindowState> &appState) : _appState(appState) {
    initializeOpenGLFunctions();
}

void Background::draw(const DrawEvent &event) {
    event.drawMethods->clear(glm::vec4(0.8, 0.8, 0.8, 1), 1);
}

void Background::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->clear(hitColor.toColor(), 1);
}

void Background::mousePressEvent(const MouseEvent &event) {
    Q_UNUSED(event)
    _appState->document()->currentScene()->setSelectedNodes({});
}

} // namespace shapecraft
