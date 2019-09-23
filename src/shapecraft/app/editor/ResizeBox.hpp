#pragma once

#include <QObject>
#include <shapecraft/render/viewport/Renderable.hpp>

namespace shapecraft {

class ResizeBox : public viewport::Renderable {
    Q_OBJECT
  public:
    ResizeBox();
};

} // namespace shapecraft
