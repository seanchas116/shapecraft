#pragma once
#include "Debug.hpp"
#include "Ray.hpp"
#include <boost/operators.hpp>
#include <glm/glm.hpp>

namespace shapecraft {

template <typename T>
class Box final : private boost::orable<Box<T>>, private boost::equality_comparable<Box<T>> {
  public:
    Box() = default;

    static Box fromSize(const glm::tvec3<T> &pos, const glm::tvec3<T> &size) {
        return Box(pos, size);
    }

    glm::tvec3<T> minPosition() const { return _position; }
    glm::tvec3<T> maxPosition() const { return _position + _size; }
    glm::tvec3<T> size() const { return _size; }

    bool intersects(const Ray<T> &ray) const {
        // https://gamedev.stackexchange.com/a/18459
        using namespace glm;

        tvec3<T> dirfrac = tvec3<T>(1) / ray.direction;

        auto minPos = minPosition();
        auto maxPos = maxPosition();

        T t1 = (minPos.x - ray.origin.x) * dirfrac.x;
        T t2 = (maxPos.x - ray.origin.x) * dirfrac.x;
        T t3 = (minPos.y - ray.origin.y) * dirfrac.y;
        T t4 = (maxPos.y - ray.origin.y) * dirfrac.y;
        T t5 = (minPos.z - ray.origin.z) * dirfrac.z;
        T t6 = (maxPos.z - ray.origin.z) * dirfrac.z;

        T tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
        T tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

        return 0 <= tmax && tmin <= tmax;
    }

    bool operator==(const Box &other) const {
        return _position == other._position && _size == other._size;
    }

    Box &operator|=(const Box &other) {
        auto minPos = glm::min(minPosition(), other.minPosition());
        auto maxPos = glm::max(maxPosition(), other.maxPosition());

        _position = minPos;
        _size = maxPos - minPos;

        return *this;
    }

  private:
    Box(glm::tvec3<T> position, glm::tvec3<T> size) : _position(position), _size(size) {}

    glm::tvec3<T> _position{0};
    glm::tvec3<T> _size{0};
};

template <typename T>
QDebug operator<<(QDebug debug, const Box<T> &box) {
    QDebugStateSaver saver(debug);
    debug.nospace();
    debug << "Box(" << box.minPosition() << ", " << box.size() << ")";
    return debug;
}

} // namespace shapecraft
