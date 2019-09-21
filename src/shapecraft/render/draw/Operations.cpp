#include "Operations.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/Texture.hpp"
#include "shapecraft/render/gl/VAO.hpp"

namespace shapecraft {

namespace draw {

namespace {

struct CopyVAOAttribute {
    glm::vec2 texCoord;
};

SP<gl::VAO> createCopyVAO() {
    std::vector<CopyVAOAttribute> attributes = {
        {glm::vec2(0, 0)},
        {glm::vec2(1, 0)},
        {glm::vec2(1, 1)},
        {glm::vec2(0, 1)},
    };

    auto vbo = std::make_shared<gl::VertexBuffer<CopyVAOAttribute>>(attributes);
    return std::make_shared<gl::VAO>(vbo, gl::Primitive::TriangleFan);
}

} // namespace

Operations::Operations()
    : _copyShader(resource::read(shaderDir + "Copy.vert"), {}, resource::read(shaderDir + "Copy.frag")),
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

} // namespace draw

} // namespace shapecraft
