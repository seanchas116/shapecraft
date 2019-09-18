#include "Util.hpp"

namespace shapecraft {
namespace viewport {

glm::dvec2 mapQtToGL(const QWidget *widget, const QPoint &p) {
    return glm::dvec2(p.x(), widget->height() - p.y());
}

} // namespace viewport
} // namespace shapecraft
