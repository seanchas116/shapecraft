#pragma once
#include "CameraController.hpp"
#include "shapecraft/Common.hpp"
#include "shapecraft/render/viewport/Viewport.hpp"
#include <QOpenGLWidget>

namespace shapecraft {

class WindowState;
class EditorScene;
class KeyObserver;
class CameraState;

class EditorViewport final : public viewport::Viewport {
    Q_OBJECT
    using super = Viewport;

  public:
    explicit EditorViewport(const SP<WindowState> &appState, const SP<KeyObserver> &keyObserver, QWidget *parent = nullptr);

    auto &cameraState() const { return _cameraState; }

  protected:
    void resizeEvent(QResizeEvent *event) override;

  private:
    SP<WindowState> _appState;
    SP<CameraState> _cameraState;
    CameraController _cameraController;
};

} // namespace shapecraft
