#pragma once
#include "../gl/Shader.hpp"

namespace shapecraft {

namespace gl {
class Texture;
class VAO;
} // namespace gl

namespace draw {

class Copy : public QOpenGLExtraFunctions {
  public:
    Copy();

    void copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity = 1);

  private:
    gl::Shader _shader;
    SP<gl::VAO> _vao;
};

} // namespace draw
} // namespace shapecraft
