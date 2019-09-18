#pragma once

#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>

namespace shapecraft {
namespace draw {

class Clear final : protected QOpenGLExtraFunctions {
  public:
    Clear();

    void clear(glm::vec4 color, float depth);
    void clearDepth(float depth);
};

} // namespace draw
} // namespace shapecraft
