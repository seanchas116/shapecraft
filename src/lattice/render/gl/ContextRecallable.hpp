#pragma once

class QOpenGLContext;
class QSurface;

namespace lattice {
namespace gl {

class ContextRecallable {
public:
    ContextRecallable();

    void recallContext();

private:
    QOpenGLContext* _context;
    QSurface* _surface;
};

}
} // namespace Lattice
