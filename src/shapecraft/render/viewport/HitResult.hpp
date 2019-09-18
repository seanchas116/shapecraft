#pragma once
#include "shapecraft/Common.hpp"
#include <glm/glm.hpp>

namespace shapecraft {
namespace viewport {

class Renderable;

struct HitResult {
    SP<Renderable> renderable;
    float depth;
};

} // namespace viewport
} // namespace shapecraft
