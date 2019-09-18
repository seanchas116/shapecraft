#pragma once
#include "../gl/Shader.hpp"

namespace shapecraft {
class Camera;

namespace gl {
class VAO;
}

namespace draw {

class DrawCircle final {
  public:
    inline static constexpr double defaultZOffset = -0.00002;

    DrawCircle();

    void draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera,
              double width, glm::vec4 color, bool useVertexColor = false, double zOffset = defaultZOffset);
    void draw2D(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, glm::ivec2 viewportSize,
                double width, glm::vec4 color, bool useVertexColor = false);

  private:
    gl::Shader _shader;
};

} // namespace draw
} // namespace shapecraft
