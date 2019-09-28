#include "OpenGLEntity.hpp"

namespace shapecraft {
namespace gl {

OpenGLEntity::OpenGLEntity() {
    initializeOpenGLFunctions();
    _context = QOpenGLContext::currentContext();
    Q_ASSERT(_context);
}

} // namespace gl
} // namespace shapecraft
