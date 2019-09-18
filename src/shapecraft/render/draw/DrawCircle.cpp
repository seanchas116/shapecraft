#include "DrawCircle.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/VAO.hpp"
#include "shapecraft/util/Camera.hpp"
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace shapecraft {
namespace draw {

DrawCircle::DrawCircle() : _shader(resource::read(shaderDir + "DrawCircle.vert"),
                                   resource::read(shaderDir + "DrawCircle.geom"),
                                   resource::read(shaderDir + "DrawCircle.frag")) {
}

void DrawCircle::draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, double width, glm::vec4 color, bool useVertexColor, double zOffset) {
    _shader.bind();
    _shader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    _shader.setUniform("viewportSize", camera.viewportSize());
    _shader.setUniform("width", width);
    _shader.setUniform("color", color);
    _shader.setUniform("useVertexColor", useVertexColor);
    _shader.setUniform("zOffset", zOffset);
    vao->draw();
}

void DrawCircle::draw2D(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, glm::ivec2 viewportSize, double width, glm::vec4 color, bool useVertexColor) {
    _shader.bind();
    dmat4 MVP = translate(dvec3(-1.0)) * scale(dvec3(2.0 / dvec2(viewportSize), 2.0)) * matrix;
    _shader.setUniform("MVP", MVP);
    _shader.setUniform("viewportSize", dvec2(viewportSize));
    _shader.setUniform("width", width);
    _shader.setUniform("color", color);
    _shader.setUniform("useVertexColor", useVertexColor);
    _shader.setUniform("zOffset", 0.0);
    vao->draw();
}

} // namespace draw
} // namespace shapecraft
