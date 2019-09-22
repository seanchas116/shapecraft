#include "Operations.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/Texture.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/util/Camera.hpp"
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace shapecraft {
namespace draw {

namespace {

struct CopyVAOAttribute {
    glm::vec2 texCoord;
};

SP<gl::VertexArray> createCopyVAO() {
    std::vector<CopyVAOAttribute> attributes = {
        {glm::vec2(0, 0)},
        {glm::vec2(1, 0)},
        {glm::vec2(1, 1)},
        {glm::vec2(0, 1)},
    };

    auto vbo = std::make_shared<gl::VertexBuffer<CopyVAOAttribute>>(attributes);
    return std::make_shared<gl::VertexArray>(vbo, gl::Primitive::TriangleFan);
}

} // namespace

Operations::Operations()
    : _copyShader(resource::read(shaderDir + "Copy.vert"), {},
                  resource::read(shaderDir + "Copy.frag")),
      _drawCircleShader(resource::read(shaderDir + "DrawCircle.vert"),
                        resource::read(shaderDir + "DrawCircle.geom"),
                        resource::read(shaderDir + "DrawCircle.frag")),
      _copyVAO(createCopyVAO()) {
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

void Operations::copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity) {
    _copyShader.bind();

    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glActiveTexture(GL_TEXTURE1);
    depthTexture->bind();

    _copyShader.setUniform("colorSampler", 0);
    _copyShader.setUniform("depthSampler", 1);
    _copyShader.setUniform("opacity", opacity);
    _copyVAO->draw();
}

void Operations::drawCircle(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera, double width, glm::vec4 color, bool useVertexColor, double zOffset) {
    _drawCircleShader.bind();
    _drawCircleShader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    _drawCircleShader.setUniform("viewportSize", camera.viewportSize());
    _drawCircleShader.setUniform("width", width);
    _drawCircleShader.setUniform("color", color);
    _drawCircleShader.setUniform("useVertexColor", useVertexColor);
    _drawCircleShader.setUniform("zOffset", zOffset);
    vao->draw();
}

void Operations::drawCircle2D(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, glm::ivec2 viewportSize, double width, glm::vec4 color, bool useVertexColor) {
    _drawCircleShader.bind();
    dmat4 MVP = translate(dvec3(-1.0)) * scale(dvec3(2.0 / dvec2(viewportSize), 2.0)) * matrix;
    _drawCircleShader.setUniform("MVP", MVP);
    _drawCircleShader.setUniform("viewportSize", dvec2(viewportSize));
    _drawCircleShader.setUniform("width", width);
    _drawCircleShader.setUniform("color", color);
    _drawCircleShader.setUniform("useVertexColor", useVertexColor);
    _drawCircleShader.setUniform("zOffset", 0.0);
    vao->draw();
}

} // namespace draw
} // namespace shapecraft
