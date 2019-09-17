#pragma once

#include "HitResult.hpp"
#include "lattice/Common.hpp"
#include "lattice/render/gl/ContextRecallable.hpp"
#include <QOpenGLExtraFunctions>
#include <QVector>
#include <QtGlobal>
#include <glm/glm.hpp>

namespace lattice {
class Camera;

namespace gl {
class Framebuffer;
}

namespace draw {
class Operations;
}

namespace Viewport {

class Renderable;
struct DrawEvent;

class HitAreaMap final : protected gl::ContextRecallable {
    Q_DISABLE_COPY(HitAreaMap)
  public:
    HitAreaMap();

    Opt<HitResult> pick(glm::vec2 physicalPos);
    void draw(const SP<Renderable> &renderable, const DrawEvent &drawEvent);

  private:
    void resize(glm::ivec2 size);

    SP<gl::Framebuffer> _framebuffer;
    glm::ivec2 _framebufferSize = {0, 0};
    std::vector<SP<Renderable>> _lastRenderables;
};

} // namespace Viewport
} // namespace lattice
