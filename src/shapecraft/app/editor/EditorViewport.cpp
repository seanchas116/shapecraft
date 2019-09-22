#include "EditorViewport.hpp"
#include "CameraState.hpp"
#include "ViewportControlView.hpp"
#include "shapecraft/util/KeyObserver.hpp"
#include <QResizeEvent>
#include <QVBoxLayout>

namespace shapecraft {

EditorViewport::EditorViewport(const SP<WindowState> &appState, const SP<KeyObserver> &keyObserver, QWidget *parent)
    : Viewport::Viewport(parent),
      _appState(appState),
      _cameraState(std::make_shared<CameraState>()),
      _cameraController(_cameraState, this) {
    connect(keyObserver.get(), &KeyObserver::pressedKeysChanged, &_cameraController, &CameraController::setPressedKeys);
    connect(_cameraState.get(), &CameraState::cameraChanged, this, &Viewport::setCamera);
    setCamera(_cameraState->camera());

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    auto controlView = new ViewportControlView(_cameraState, this);
    layout->addWidget(controlView);

    setLayout(layout);
}

void EditorViewport::resizeEvent(QResizeEvent *event) {
    super::resizeEvent(event);
    _cameraState->setViewportSize(glm::vec2(event->size().width(), event->size().height()));
}

} // namespace shapecraft
