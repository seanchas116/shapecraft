#pragma once

#include "./Clear.hpp"
#include "./Copy.hpp"
#include "./DrawCircle.hpp"
#include "./DrawLine.hpp"
#include "./DrawMaterial.hpp"
#include "./DrawUnicolor.hpp"

namespace shapecraft {
namespace draw {

class Operations final : protected QOpenGLExtraFunctions {
  public:
    Operations();

    void clear(glm::vec4 color, float depth);
    void clearDepth(float depth);

    void copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity = 1);

    DrawCircle drawCircle;
    DrawLine drawLine;
    DrawMaterial drawMaterial;
    DrawUnicolor drawUnicolor;

  private:
    gl::Shader _copyShader;
    SP<gl::VAO> _copyVAO;
};

} // namespace draw
} // namespace shapecraft
