#pragma once

#include "shapecraft/Common.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/viewport/Renderable.hpp"
#include <glm/glm.hpp>

namespace shapecraft {

namespace gl {
template <typename T>
class VertexBuffer;
class IndexBuffer;
} // namespace gl

class GridFloor final : public viewport::Renderable {
    Q_OBJECT
  public:
    GridFloor();

    void draw(const viewport::DrawEvent &event) override;

  private:
    SP<gl::VertexArray> _vao;
    SP<gl::VertexArray> _yAxisVAO;
    SP<gl::VertexArray> _zAxisVAO;
};

} // namespace shapecraft
