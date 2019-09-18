#pragma once
#include "shapecraft/render/viewport/ViewportContainer.hpp"
#include <QWidget>

namespace shapecraft {

class EditorViewportContainer : public viewport::ViewportContainer {
    Q_OBJECT
  public:
    explicit EditorViewportContainer(QWidget *parent = nullptr);
};

} // namespace shapecraft
