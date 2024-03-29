#pragma once

#include <array>
#include <glm/glm.hpp>
#include <tuple>

namespace shapecraft {

template <typename T>
class Ray final {
  public:
    Ray() = default;
    Ray(glm::tvec3<T> origin, glm::tvec3<T> direction) : origin(origin), direction(direction) {}

    template <typename U>
    Ray(const Ray<U> &other) : origin(other.origin), direction(other.direction) {}

    std::tuple<bool, float> intersectsTriangle(const std::array<glm::tvec3<T>, 3> &triangle) const {
        // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
        using namespace glm;
        const T EPSILON = 0.0000001;
        auto [vertex0, vertex1, vertex2] = triangle;
        tvec3<T> edge1, edge2, h, s, q;
        T a, f, u, v;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        h = cross(direction, edge2);
        a = dot(edge1, h);
        if (a > -EPSILON && a < EPSILON) {
            return {false, 0}; // This ray is parallel to this triangle.
        }
        f = 1 / a;
        s = origin - vertex0;
        u = f * (dot(s, h));
        if (u < 0.0 || u > 1.0) {
            return {false, 0};
        }
        q = cross(s, edge1);
        v = f * dot(direction, q);
        if (v < 0.0 || u + v > 1.0) {
            return {false, 0};
        }
        // At this stage we can compute t to find out where the intersection point is on the line.
        T t = f * dot(edge2, q);
        if (t > EPSILON) {
            return {true, t};
        } else {
            return {false, 0};
        }
    }

    glm::tvec3<T> at(T t) const {
        return origin + direction * t;
    }

    // planeIntercept(0) -> find point in Ray where x == 0
    // planeIntercept(1) -> find point in Ray where y == 0
    // planeIntercept(2) -> find point in Ray where z == 0
    glm::tvec3<T> planeIntercept(int normalAxis) const {
        glm::tvec3<T> p0 = origin;
        glm::tvec3<T> p1 = direction + origin;
        int i = normalAxis;
        int j = (i + 1) % 3;
        int k = (i + 2) % 3;

        glm::tvec2<T> st = (p1[i] * glm::tvec2<T>(p0[j], p0[k]) - p0[i] * glm::tvec2<T>(p1[j], p1[k])) /
                           (p1[i] - p0[i]);

        glm::tvec3<T> ret;
        ret[i] = 0;
        ret[j] = st.x;
        ret[k] = st.y;
        return ret;
    }

    glm::tvec3<T> origin{0};
    glm::tvec3<T> direction{0};
};

template <typename T>
Ray<T> operator*(const glm::tmat4x4<T> &mat, const Ray<T> &ray) {
    return {
        (mat * glm::dvec4(ray.origin, 1)).xyz,
        (mat * glm::dvec4(ray.direction, 0)).xyz,
    };
}

} // namespace shapecraft
