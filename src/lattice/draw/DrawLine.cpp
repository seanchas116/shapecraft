#include "DrawLine.hpp"
#include "Constants.hpp"
#include "lattice/Resource.hpp"
#include "lattice/gl/VAO.hpp"
#include "lattice/util/Camera.hpp"

namespace lattice {
namespace draw {

DrawLine::DrawLine() : _shader(resource::read(shaderDir + "DrawLine.vert"),
                               resource::read(shaderDir + "DrawLine.geom"),
                               resource::read(shaderDir + "DrawLine.frag")) {
}

void DrawLine::draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, double width, glm::vec4 color, bool useVertexColor, double zOffset) {
    _shader.bind();
    _shader.setUniform("MV", camera.worldToCameraMatrix() * matrix);
    _shader.setUniform("P", camera.cameraToViewportMatrix());
    _shader.setUniform("viewportSize", camera.viewportSize());
    _shader.setUniform("zNear", camera.projection() == Camera::Projection::Perspective ? camera.zNear() : -10000.0); // TODO: specify depth in better way
    _shader.setUniform("width", width);
    _shader.setUniform("color", color);
    _shader.setUniform("useVertexColor", useVertexColor);
    _shader.setUniform("zOffset", zOffset);
    vao->draw();
}

} // namespace draw
} // namespace lattice
