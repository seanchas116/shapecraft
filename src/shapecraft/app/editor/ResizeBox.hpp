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
    void updateVertexArray();

    Box<double> _box;
    SP<gl::VertexArray> _vertexArray;
};

} // namespace shapecraft
