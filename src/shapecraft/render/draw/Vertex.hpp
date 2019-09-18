#pragma once
#include <glm/glm.hpp>

namespace shapecraft {
namespace draw {

struct Vertex {
    glm::vec3 position{0};
    glm::vec2 texCoord{0};
    glm::vec3 normal{0};
    glm::vec4 color{0};
};

struct PointLineVertex {
    glm::vec3 position{0};
    glm::vec4 color{0};
    float width{1};
};

} // namespace draw
} // namespace shapecraft
