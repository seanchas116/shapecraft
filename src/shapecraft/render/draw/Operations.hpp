#pragma once

#include "../gl/Shader.hpp"
#include "./Clear.hpp"
#include "./Copy.hpp"
#include "./DrawCircle.hpp"
#include "./DrawLine.hpp"
#include "./DrawMaterial.hpp"
#include "./DrawUnicolor.hpp"
#include <QOpenGLExtraFunctions>

namespace shapecraft {

namespace gl {
class Texture;
class VertexArray;
} // namespace gl

namespace draw {

class Operations final : protected QOpenGLExtraFunctions {
  public:
    Operations();

    void clear(glm::vec4 color, float depth);
    void clearDepth(float depth);

    void copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity = 1);

    inline static constexpr double defaultCircleZOffset = -0.00002;

    void drawCircle(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera,
                    double width, glm::vec4 color, bool useVertexColor = false, double zOffset = defaultCircleZOffset);
    void drawCircle2D(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, glm::ivec2 viewportSize,
                      double width, glm::vec4 color, bool useVertexColor = false);

    inline static constexpr double defaultLineZOffset = -0.00001;

    void drawLine(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera,
                  double width, glm::vec4 color, bool useVertexColor = false, double zOffset = defaultLineZOffset);

    DrawMaterial drawMaterial;
    DrawUnicolor drawUnicolor;

  private:
    gl::Shader _copyShader;
    gl::Shader _drawCircleShader;
    gl::Shader _drawLineShader;

    SP<gl::VertexArray> _copyVAO;
};

} // namespace draw
} // namespace shapecraft
