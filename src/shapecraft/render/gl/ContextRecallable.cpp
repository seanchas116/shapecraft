#include "ContextRecallable.hpp"
#include <QOpenGLContext>

namespace shapecraft {
namespace gl {

ContextRecallable::ContextRecallable() {
    _context = QOpenGLContext::currentContext();
    _surface = _context->surface();
}

void ContextRecallable::recallContext() {
    _context->makeCurrent(_surface);
}

} // namespace gl
} // namespace shapecraft
