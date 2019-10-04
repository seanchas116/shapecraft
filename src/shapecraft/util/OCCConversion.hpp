#pragma once

#include <glm/glm.hpp>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

namespace shapecraft {

inline glm::mat4 toGLM(const gp_Trsf &t) {
    return glm::mat4(t.Value(1, 1), t.Value(2, 1), t.Value(3, 1), 0,
                     t.Value(1, 2), t.Value(2, 2), t.Value(3, 2), 0,
                     t.Value(1, 3), t.Value(2, 3), t.Value(3, 3), 0,
                     t.Value(1, 4), t.Value(2, 4), t.Value(3, 4), 1);
}

inline gp_Trsf toOCC(const glm::mat4 &fmat) {
    glm::dmat4 t = fmat;
    gp_Trsf ret;
    ret.SetValues(t[0][0], t[1][0], t[2][0], t[3][0],
                  t[0][1], t[1][1], t[2][1], t[3][1],
                  t[0][2], t[1][2], t[2][2], t[3][2]);
    return ret;
}

inline glm::vec3 toGLM(const gp_Pnt &p) {
    return glm::vec3(p.X(), p.Y(), p.Z());
}

inline gp_Pnt toOCC(const glm::vec3 &p) {
    return gp_Pnt(double(p.x), double(p.y), double(p.z));
}

} // namespace shapecraft