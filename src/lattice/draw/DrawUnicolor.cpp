#include "DrawUnicolor.hpp"
#include "Constants.hpp"
#include "lattice/Resource.hpp"
#include "lattice/gl/VAO.hpp"
#include "lattice/util/Camera.hpp"

namespace lattice {
namespace draw {

DrawUnicolor::DrawUnicolor() : _shader(resource::read(shaderDir + "DrawUnicolor.vert"),
                                       std::string(),
                                       resource::read(shaderDir + "DrawUnicolor.frag")) {
}

void DrawUnicolor::draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, glm::vec4 color, bool useVertexColor) {
    _shader.bind();
    _shader.setUniform("color", color);
    _shader.setUniform("useVertexColor", useVertexColor);
    _shader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    vao->draw();
}

} // namespace draw
} // namespace lattice
