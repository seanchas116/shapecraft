#pragma once
#include <QOpenGLExtraFunctions>

namespace shapecraft {
namespace gl {

class OpenGLEntity : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(OpenGLEntity)
  public:
    OpenGLEntity();

    QOpenGLContext *context() const { return _context; }

  private:
    QOpenGLContext *_context;
};

} // namespace gl
} // namespace shapecraft
