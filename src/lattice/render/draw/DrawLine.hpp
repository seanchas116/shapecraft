#pragma once

#include "../gl/Shader.hpp"

namespace lattice {
class Camera;

namespace gl {
class VAO;
}

namespace draw {

class DrawLine final {
  public:
    inline static constexpr double defaultZOffset = -0.00001;

    DrawLine();

    void draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera,
              double width, glm::vec4 color, bool useVertexColor = false, double zOffset = defaultZOffset);

  private:
    gl::Shader _shader;
};

} // namespace draw
} // namespace lattice
