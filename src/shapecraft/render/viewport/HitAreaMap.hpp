#pragma once

#include "HitResult.hpp"
#include "Renderable.hpp"
#include "shapecraft/Common.hpp"
#include "shapecraft/render/gl/ContextRecallable.hpp"
#include <QOpenGLExtraFunctions>
#include <QVector>
#include <QtGlobal>
#include <glm/glm.hpp>

namespace shapecraft {
class Camera;

namespace gl {
class Framebuffer;
}

namespace draw {
class DrawMethods;
}

namespace viewport {

class HitAreaMap final : protected gl::ContextRecallable {
    Q_DISABLE_COPY(HitAreaMap)
  public:
    HitAreaMap();

    Opt<HitResult> pick(glm::vec2 physicalPos);
    void draw(const SP<Renderable> &renderable, const Renderable::DrawEvent &drawEvent);

  private:
    void resize(glm::ivec2 size);

    SP<gl::Framebuffer> _framebuffer;
    glm::ivec2 _framebufferSize = {0, 0};
    std::vector<SP<Renderable>> _lastRenderables;
};

} // namespace viewport
} // namespace shapecraft
