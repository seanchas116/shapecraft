#pragma once

#include "Ray.hpp"
#include <array>
#include <glm/glm.hpp>
#include <tuple>

namespace shapecraft {

template <typename T>
class RayPointDistanceSolver final {
  public:
    RayPointDistanceSolver(const Ray<T> &l, const glm::tvec3<T> p) {
        // http://geomalgorithms.com/a02-_lines.html#Distance-to-Infinite-Line
        using namespace glm;
        tvec3<T> w = p - l.origin;
        tvec3<T> v = glm::normalize(l.direction);
        distance = length(cross(v, w));
        t = dot(l.direction, w) / glm::dot(l.direction, l.direction);
    }
    T t;
    T distance;
};

template <typename T>
class RayRayDistanceSolver final {
  public:
    RayRayDistanceSolver(const Ray<T> &l0, const Ray<T> &l1) {
        // http://geomalgorithms.com/a07-_distance.html
        using namespace glm;
        tvec3<T> u = l0.direction;
        tvec3<T> v = l1.direction;
        tvec3<T> w0 = l0.origin - l1.origin;

        T a = dot(u, u);
        T b = dot(u, v);
        T c = dot(v, v);
        T d = dot(u, w0);
        T e = dot(v, w0);

        T denom = a * c - b * b;

        t0 = (b * e - c * d) / denom;
        t1 = (a * e - b * d) / denom;

        tvec3<T> p = l0.origin + l0.direction * t0;
        tvec3<T> q = l1.origin + l1.direction * t1;

        distance = glm::distance(p, q);
    }
    T t0;
    T t1;
    T distance;
};

} // namespace shapecraft
