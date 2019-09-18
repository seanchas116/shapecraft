#pragma once
#include "shapecraft/Common.hpp"
#include <glm/glm.hpp>

namespace shapecraft {
namespace Viewport {

class Renderable;

struct HitResult {
    SP<Renderable> renderable;
    float depth;
};

} // namespace Viewport
} // namespace shapecraft
