#pragma once
#include "shapecraft/Common.hpp"
#include <QWidget>

namespace shapecraft {

class CameraState;

class ViewportControlView : public QWidget {
    Q_OBJECT
  public:
    explicit ViewportControlView(const SP<CameraState> &cameraState, QWidget *parent = nullptr);

  private:
    SP<CameraState> _cameraState;
};

} // namespace shapecraft
