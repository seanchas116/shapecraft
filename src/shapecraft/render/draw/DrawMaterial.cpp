#include "DrawMaterial.hpp"
#include "Constants.hpp"
#include "shapecraft/Resource.hpp"
#include "shapecraft/render/gl/Texture.hpp"
#include "shapecraft/render/gl/VAO.hpp"
#include "shapecraft/util/Camera.hpp"

namespace shapecraft {
namespace draw {

DrawMaterial::DrawMaterial() : _shader(resource::read(shaderDir + "DrawMaterial.vert"),
                                       std::string(),
                                       resource::read(shaderDir + "DrawMaterial.frag")) {
    initializeOpenGLFunctions();
}

void DrawMaterial::draw(const SP<gl::VAO> &vao, const glm::dmat4 &matrix, const Camera &camera, const Material &material) {
    _shader.bind();
    _shader.setUniform("diffuse", material.baseColor);
    _shader.setUniform("ambient", glm::vec3(0));
    _shader.setUniform("MV", camera.worldToCameraMatrix() * matrix);
    _shader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);

    if (!material.baseColorImage.isNull()) {
        auto texture = getTexture(material.baseColorImage);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        _shader.setUniform("diffuseTexture", 0.0);
        _shader.setUniform("hasDiffuseTexture", true);
    } else {
        _shader.setUniform("hasDiffuseTexture", false);
    }

    vao->draw();
}

SP<gl::Texture> DrawMaterial::getTexture(const QImage &image) {
    if (auto it = _textures.find(image.cacheKey()); it != _textures.end()) {
        return it->second;
    }

    auto rgbaTexture = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
    rgbaTexture = rgbaTexture.mirrored();
    glm::ivec2 size(rgbaTexture.width(), rgbaTexture.height());

    auto texture = std::make_shared<gl::Texture>(size, gl::Texture::Format::RGBA8, rgbaTexture.bits());
    _textures.insert({image.cacheKey(), texture});
    return texture;
}

} // namespace draw
} // namespace shapecraft
