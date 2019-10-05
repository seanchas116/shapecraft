#pragma once

#include <glm/glm.hpp>
#include <gp_GTrsf.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

namespace shapecraft {

inline glm::dvec3 toVec3(const gp_Pnt &p) {
    return glm::dvec3(p.X(), p.Y(), p.Z());
}

inline glm::dmat4 toMat4(const gp_Trsf &t) {
    return glm::dmat4(t.Value(1, 1), t.Value(2, 1), t.Value(3, 1), 0,
                      t.Value(1, 2), t.Value(2, 2), t.Value(3, 2), 0,
                      t.Value(1, 3), t.Value(2, 3), t.Value(3, 3), 0,
                      t.Value(1, 4), t.Value(2, 4), t.Value(3, 4), 1);
}

inline gp_Pnt toPnt(const glm::dvec3 &p) {
    return gp_Pnt(p.x, p.y, p.z);
}

inline gp_XYZ toXYZ(const glm::dvec3 &p) {
    return gp_XYZ(p.x, p.y, p.z);
}

inline gp_Mat toMat(const glm::dmat3 &t) {
    return gp_Mat(t[0][0], t[1][0], t[2][0],
                  t[0][1], t[1][1], t[2][1],
                  t[0][2], t[1][2], t[2][2]);
}

inline gp_Trsf toTrsf(const glm::dmat4 &t) {
    gp_Trsf ret;
    ret.SetValues(t[0][0], t[1][0], t[2][0], t[3][0],
                  t[0][1], t[1][1], t[2][1], t[3][1],
                  t[0][2], t[1][2], t[2][2], t[3][2]);
    return ret;
}

inline gp_GTrsf toGTrsf(const glm::dmat4 &t) {
    auto mat = toMat(glm::dmat3(t));
    auto v = toXYZ(t[3].xyz);
    return gp_GTrsf(mat, v);
}

} // namespace shapecraft
