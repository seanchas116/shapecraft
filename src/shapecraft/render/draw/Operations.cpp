#include "Operations.hpp"

namespace shapecraft {
namespace draw {

Operations::Operations() {
    initializeOpenGLFunctions();
}

void Operations::clear(glm::vec4 color, float depth) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepthf(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Operations::clearDepth(float depth) {
    glClearDepthf(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

} // namespace draw
} // namespace shapecraft
