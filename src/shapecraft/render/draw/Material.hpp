#pragma once
#include <QImage>
#include <glm/glm.hpp>

namespace shapecraft {
namespace draw {

struct Material {
    glm::vec3 baseColor{1, 0, 0};
    QImage baseColorImage;

    float metallic{0};
    QImage metallicImage;

    float roughness{0.5};
    QImage roughnessImage;
};

} // namespace draw
} // namespace shapecraft
