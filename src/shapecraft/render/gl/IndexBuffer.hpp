#pragma once
#include <QOpenGLExtraFunctions>
#include <array>

namespace shapecraft {
namespace gl {

enum class Primitive {
    Point,
    Line,
    Triangle,
    TriangleFan,
};

class IndexBuffer : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(IndexBuffer)
  public:
    using Triangle = std::array<uint32_t, 3>;
    using LineStrip = std::vector<uint32_t>;
    using Line = std::array<uint32_t, 2>;

    IndexBuffer();
    IndexBuffer(const std::vector<Triangle> &triangles) : IndexBuffer() { setTriangles(triangles); }
    IndexBuffer(const std::vector<Line> &lines) : IndexBuffer() { setLines(lines); }
    IndexBuffer(const std::vector<LineStrip> &strips) : IndexBuffer() { setLineStrips(strips); }

    virtual ~IndexBuffer();

    void bind();
    void unbind();

    void setTriangles(const std::vector<Triangle> &triangles);
    void setLines(const std::vector<Line> &lines);
    void setLineStrips(const std::vector<LineStrip> &strips);

    size_t size() const { return _size; }
    Primitive primitive() const { return _primitive; }

  protected:
    void setData(const void *data, size_t indexCount);

  private:
    Primitive _primitive = Primitive::Triangle;
    GLuint _buffer = 0;
    size_t _size = 0;
};

} // namespace gl
} // namespace shapecraft
