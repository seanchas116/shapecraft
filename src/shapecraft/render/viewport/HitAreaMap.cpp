#include "HitAreaMap.hpp"
#include "../gl/Framebuffer.hpp"
#include "../gl/Texture.hpp"
#include "Renderable.hpp"
#include "shapecraft/util/Camera.hpp"
#include "shapecraft/util/Debug.hpp"
#include <glm/gtc/type_precision.hpp>

using namespace glm;

namespace shapecraft {
namespace viewport {

HitAreaMap::HitAreaMap() : _framebuffer(std::make_shared<gl::Framebuffer>(glm::ivec2(0, 0))) {
}

void HitAreaMap::resize(glm::ivec2 size) {
    if (size == _framebufferSize) {
        return;
    }
    _framebuffer = std::make_shared<gl::Framebuffer>(size,
                                                     std::vector{std::make_shared<gl::Texture>(size, gl::Texture::Format::RGBA32F)},
                                                     std::make_shared<gl::Texture>(size, gl::Texture::Format::Depth24Stencil8));

    _framebufferSize = size;
}

Opt<HitResult> HitAreaMap::pick(vec2 physicalPos) {
    recallContext();
    PixelData<vec4> pixels(glm::ivec2(1));
    _framebuffer->readPixels(physicalPos, pixels);

    auto renderable = Renderable::fromIDColor(pixels.data()[0]);

    if (!renderable) {
        return {};
    }
    float depth = _framebuffer->readDepth(physicalPos);
    return {{*renderable, depth}};
}

void HitAreaMap::draw(const SP<Renderable> &renderable, const DrawEvent &drawEvent) {
    resize(drawEvent.camera.viewportSize());

    _framebuffer->bind();
    drawEvent.drawMethods->clear(glm::vec4(0), 1);
    renderable->drawHitAreaRecursive(drawEvent);
    _framebuffer->unbind();

    _lastRenderables.clear();
    renderable->getDescendants(_lastRenderables);
}

} // namespace viewport
} // namespace shapecraft
