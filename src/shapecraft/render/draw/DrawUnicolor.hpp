#pragma once
#include "../gl/Shader.hpp"

namespace shapecraft {
class Camera;

namespace gl {
class VAO;
}

namespace draw {

class DrawUnicolor final {
  public:
    DrawUnicolor();

    void draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, glm::vec4 color, bool useVertexColor = false);

  private:
    gl::Shader _shader;
};

} // namespace draw
} // namespace shapecraft
