#include "DrawUnicolor.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/util/Camera.hpp"

namespace shapecraft {
namespace draw {

DrawUnicolor::DrawUnicolor() : _shader(resource::read(shaderDir + "DrawUnicolor.vert"),
                                       std::string(),
                                       resource::read(shaderDir + "DrawUnicolor.frag")) {
}

void DrawUnicolor::draw(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera, glm::vec4 color, bool useVertexColor) {
    _shader.bind();
    _shader.setUniform("color", color);
    _shader.setUniform("useVertexColor", useVertexColor);
    _shader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    vao->draw();
}

} // namespace draw
} // namespace shapecraft
