#pragma once

#include "shapecraft/render/gl/ContextRecallable.hpp"
#include "shapecraft/render/viewport/Renderable.hpp"
#include "shapecraft/util/Box.hpp"
#include <QObject>

namespace shapecraft {

class ResizeBox : public viewport::Renderable, protected gl::ContextRecallable {
    Q_OBJECT
  public:
    ResizeBox();

    void setBox(const Box<double> &box);

    void draw(const DrawEvent &event) override;

  private:
    void updateVAO();

    Box<double> _box;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;
};

} // namespace shapecraft
