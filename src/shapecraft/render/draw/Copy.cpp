#include "Copy.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/Texture.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"

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

Copy::Copy() : _shader(resource::read(shaderDir + "Copy.vert"), {}, resource::read(shaderDir + "Copy.frag")),
               _vao(createCopyVAO()) {
    initializeOpenGLFunctions();
}

void Copy::copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity) {
    _shader.bind();

    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glActiveTexture(GL_TEXTURE1);
    depthTexture->bind();

    _shader.setUniform("colorSampler", 0);
    _shader.setUniform("depthSampler", 1);
    _shader.setUniform("opacity", opacity);
    _vao->draw();
}

} // namespace draw
} // namespace shapecraft
