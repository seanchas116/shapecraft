#pragma once

#include "./Clear.hpp"
#include "./Copy.hpp"
#include "./DrawCircle.hpp"
#include "./DrawLine.hpp"
#include "./DrawMaterial.hpp"
#include "./DrawUnicolor.hpp"
#include <QOpenGLExtraFunctions>

namespace shapecraft {
namespace draw {

class Operations final : protected QOpenGLExtraFunctions {
  public:
    Operations();

    Clear clear;
    Copy copy;
    DrawCircle drawCircle;
    DrawLine drawLine;
    DrawMaterial drawMaterial;
    DrawUnicolor drawUnicolor;
};

} // namespace draw
} // namespace shapecraft
