#pragma once

#include <QObject>
#include <shapecraft/render/viewport/Renderable.hpp>
#include <shapecraft/util/Box.hpp>

namespace shapecraft {

class ResizeBox : public viewport::Renderable {
    Q_OBJECT
  public:
    ResizeBox();

    void setBox(const Box<double> &box);

  private:
    Box<double> _box;
};

} // namespace shapecraft
