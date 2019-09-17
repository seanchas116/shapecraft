#pragma once
#include "lattice/Common.hpp"
#include <glm/glm.hpp>

namespace lattice {
namespace Viewport {

class Renderable;

struct HitResult {
    SP<Renderable> renderable;
    float depth;
};

} // namespace Viewport
} // namespace lattice
