#pragma once
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "shapecraft/Common.hpp"
#include <QOpenGLExtraFunctions>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace shapecraft {
namespace gl {

enum class BufferType {
    PerVertex,
    PerInstance,
};

class VertexArray final : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(VertexArray)
  public:
    VertexArray();
    VertexArray(const SP<AnyVertexBuffer> &buffer, const SP<IndexBuffer> &indexBuffer);
    VertexArray(const SP<AnyVertexBuffer> &buffer, Primitive primitive);
    VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, const SP<IndexBuffer> &indexBuffer);
    VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, Primitive primitive);
    ~VertexArray();

    auto &buffers() const { return _buffers; }

    void draw();

  private:
    VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, const Opt<SP<IndexBuffer>> &indexBuffer, Primitive primitive);

    std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> _buffers;
    Opt<SP<IndexBuffer>> _indexBuffer;
    Primitive _primitive;
    GLuint _vertexArray = 0;
};

} // namespace gl
} // namespace shapecraft
