#include "Location.hpp"
#include "JSON.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <nlohmann/json.hpp>

using namespace glm;

namespace shapecraft {

dmat4 Location::matrixToWorld() const {
    return glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);
}

glm::dvec3 Location::right() const {
    return matrixToWorld()[0].xyz;
}

glm::dvec3 Location::up() const {
    return matrixToWorld()[1].xyz;
}

glm::dvec3 Location::backward() const {
    return matrixToWorld()[2].xyz;
}

bool Location::operator==(const Location &other) const {
    return position == other.position && scale == other.scale && rotation == other.rotation;
}

bool Location::operator!=(const Location &other) const {
    return !operator==(other);
}

void to_json(nlohmann::json &json, const Location &location) {
    json["position"] = location.position;
    json["scale"] = location.scale;
    json["rotation"] = location.rotation;
}

void from_json(const nlohmann::json &json, Location &location) {
    location.position = json["position"];
    location.scale = json["scale"];
    location.rotation = json["rotation"];
}

} // namespace shapecraft
