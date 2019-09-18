#include <QWidget>
#include <glm/glm.hpp>

namespace shapecraft {
namespace viewport {

glm::dvec2 mapQtToGL(const QWidget *widget, const QPoint &p);

} // namespace viewport
} // namespace shapecraft
