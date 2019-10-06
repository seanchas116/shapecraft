#include "DrawMethods.hpp"
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
    return std::make_shared<gl::VertexArray>(vbo, gl::IndexBuffer::Primitive::TriangleFan);
}

const std::string shaderDir = "src/shapecraft/render/draw/";

} // namespace

DrawMethods::DrawMethods()
    : _copyShader(resource::read(shaderDir + "Copy.vert"), {},
                  resource::read(shaderDir + "Copy.frag")),
      _drawCircleShader(resource::read(shaderDir + "DrawCircle.vert"),
                        resource::read(shaderDir + "DrawCircle.geom"),
                        resource::read(shaderDir + "DrawCircle.frag")),
      _drawLineShader(resource::read(shaderDir + "DrawLine.vert"),
                      resource::read(shaderDir + "DrawLine.geom"),
                      resource::read(shaderDir + "DrawLine.frag")),
      _drawMaterialShader(resource::read(shaderDir + "DrawMaterial.vert"), {},
                          resource::read(shaderDir + "DrawMaterial.frag")),
      _drawUnicolorShader(resource::read(shaderDir + "DrawUnicolor.vert"), {},
                          resource::read(shaderDir + "DrawUnicolor.frag")),
      _copyVAO(createCopyVAO()) {
}

void DrawMethods::clear(glm::vec4 color, float depth) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepthf(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawMethods::clearDepth(float depth) {
    glClearDepthf(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DrawMethods::copy(const SP<gl::Texture> &texture, const SP<gl::Texture> &depthTexture, float opacity) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == texture->context());
    Q_ASSERT(context() == depthTexture->context());

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

void DrawMethods::drawCircle(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera, double width, glm::vec4 color, bool useVertexColor, double zOffset) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == vao->context());

    _drawCircleShader.bind();
    _drawCircleShader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    _drawCircleShader.setUniform("viewportSize", camera.viewportSize());
    _drawCircleShader.setUniform("width", width);
    _drawCircleShader.setUniform("color", color);
    _drawCircleShader.setUniform("useVertexColor", useVertexColor);
    _drawCircleShader.setUniform("zOffset", zOffset);
    vao->draw();
}

void DrawMethods::drawCircle2D(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, glm::ivec2 viewportSize, double width, glm::vec4 color, bool useVertexColor) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == vao->context());

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

void DrawMethods::drawLine(const SP<gl::VertexArray> &vao, const glm::dmat4 &matrix, const Camera &camera, double width, glm::vec4 color, bool useVertexColor, double zOffset) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == vao->context());

    _drawLineShader.bind();
    _drawLineShader.setUniform("MV", camera.worldToCameraMatrix() * matrix);
    _drawLineShader.setUniform("P", camera.cameraToViewportMatrix());
    _drawLineShader.setUniform("viewportSize", camera.viewportSize());
    _drawLineShader.setUniform("zNear", camera.projection() == Camera::Projection::Perspective ? camera.zNear() : -10000.0); // TODO: specify depth in better way
    _drawLineShader.setUniform("width", width);
    _drawLineShader.setUniform("color", color);
    _drawLineShader.setUniform("useVertexColor", useVertexColor);
    _drawLineShader.setUniform("zOffset", zOffset);
    vao->draw();
}

void DrawMethods::drawMaterial(const SP<gl::VertexArray> &vao, const dmat4 &matrix, const Camera &camera, const Material &material) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == vao->context());

    _drawMaterialShader.bind();
    _drawMaterialShader.setUniform("diffuse", material.baseColor);
    _drawMaterialShader.setUniform("ambient", glm::vec3(0));
    _drawMaterialShader.setUniform("MV", camera.worldToCameraMatrix() * matrix);
    _drawMaterialShader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);

    if (!material.baseColorImage.isNull()) {
        auto texture = textureForImage(material.baseColorImage);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        _drawMaterialShader.setUniform("diffuseTexture", 0.0);
        _drawMaterialShader.setUniform("hasDiffuseTexture", true);
    } else {
        _drawMaterialShader.setUniform("hasDiffuseTexture", false);
    }

    vao->draw();
}

void DrawMethods::drawUnicolor(const SP<gl::VertexArray> &vao, const dmat4 &matrix, const Camera &camera, vec4 color, bool useVertexColor) {
    Q_ASSERT(context() == QOpenGLContext::currentContext());
    Q_ASSERT(context() == vao->context());

    _drawUnicolorShader.bind();
    _drawUnicolorShader.setUniform("color", color);
    _drawUnicolorShader.setUniform("useVertexColor", useVertexColor);
    _drawUnicolorShader.setUniform("MVP", camera.worldToViewportMatrix() * matrix);
    vao->draw();
}

SP<gl::Texture> DrawMethods::textureForImage(const QImage &image) {
    if (auto it = _imageTextureCaches.find(image.cacheKey()); it != _imageTextureCaches.end()) {
        return it->second;
    }

    auto rgbaTexture = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
    rgbaTexture = rgbaTexture.mirrored();
    glm::ivec2 size(rgbaTexture.width(), rgbaTexture.height());

    auto texture = std::make_shared<gl::Texture>(size, gl::Texture::Format::RGBA8, rgbaTexture.bits());
    _imageTextureCaches.insert({image.cacheKey(), texture});
    return texture;
}

} // namespace draw
} // namespace shapecraft
