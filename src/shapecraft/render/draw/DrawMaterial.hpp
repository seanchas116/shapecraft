#pragma once
#include "../gl/Shader.hpp"
#include "Material.hpp"
#include <QOpenGLExtraFunctions>
#include <unordered_map>

namespace shapecraft {
class Camera;

namespace gl {
class VertexArray;
class Texture;
} // namespace gl

namespace draw {

class DrawMaterial final : protected QOpenGLExtraFunctions {
  public:
    DrawMaterial();

    void draw(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera, const Material &material);

    SP<gl::Texture> getTexture(const QImage &image);

  private:
    gl::Shader _shader;
    std::unordered_map<qint64, SP<gl::Texture>> _textures;
};

} // namespace draw
} // namespace shapecraft
