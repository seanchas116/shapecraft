#include <QWidget>
#include <glm/glm.hpp>

namespace shapecraft {
namespace Viewport {

glm::dvec2 mapQtToGL(const QWidget *widget, const QPoint &p);

} // namespace Viewport
} // namespace shapecraft
