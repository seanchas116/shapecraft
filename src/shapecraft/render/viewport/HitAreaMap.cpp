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

    auto hitColor = HitColor::fromColor(pixels.data()[0]);
    if (hitColor.index >= _lastRenderables.size()) {
        return {};
    }
    auto renderable = _lastRenderables[hitColor.index];
    float depth = _framebuffer->readDepth(physicalPos);
    return {{renderable, hitColor.innerID, depth}};
}

void HitAreaMap::draw(const SP<Renderable> &renderable, const Renderable::DrawEvent &drawEvent) {
    resize(drawEvent.camera.viewportSize());

    _framebuffer->bind();
    drawEvent.drawMethods->clear(HitColor().toColor(), 1);

    _lastRenderables.clear();
    renderable->drawHitAreaRecursive(drawEvent, _lastRenderables);
    _framebuffer->unbind();
}

} // namespace viewport
} // namespace shapecraft
