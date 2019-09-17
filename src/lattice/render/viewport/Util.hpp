#include <QWidget>
#include <glm/glm.hpp>

namespace lattice {
namespace Viewport {

glm::dvec2 mapQtToGL(const QWidget* widget, const QPoint& p);

} // namespace Render
} // namespace Lattice
