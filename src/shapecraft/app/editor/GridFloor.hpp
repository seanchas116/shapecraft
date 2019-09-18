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
    SP<gl::VertexBuffer<draw::PointLineVertex>> _vbo;
    SP<gl::IndexBuffer> _indexBuffer;
    SP<gl::VAO> _vao;
    SP<gl::IndexBuffer> _yAxisIndexBuffer;
    SP<gl::IndexBuffer> _zAxisIndexBuffer;
    SP<gl::VAO> _yAxisVAO;
    SP<gl::VAO> _zAxisVAO;
};

} // namespace shapecraft
