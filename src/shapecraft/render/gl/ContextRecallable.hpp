#pragma once

class QOpenGLContext;
class QSurface;

namespace shapecraft {
namespace gl {

class ContextRecallable {
  public:
    ContextRecallable();

    void recallContext();

  private:
    QOpenGLContext *_context;
    QSurface *_surface;
};

} // namespace gl
} // namespace shapecraft
