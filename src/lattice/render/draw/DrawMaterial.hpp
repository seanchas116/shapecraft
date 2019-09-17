#pragma once
#include "../gl/Shader.hpp"
#include "Material.hpp"
#include <QOpenGLExtraFunctions>
#include <unordered_map>

namespace lattice {
class Camera;

namespace gl {
class VAO;
class Texture;
} // namespace gl

namespace draw {

class DrawMaterial final : protected QOpenGLExtraFunctions {
  public:
    DrawMaterial();

    void draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, const Material &material);

    SP<gl::Texture> getTexture(const QImage &image);

  private:
    gl::Shader _shader;
    std::unordered_map<qint64, SP<gl::Texture>> _textures;
};

} // namespace draw
} // namespace lattice
