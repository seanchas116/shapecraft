#pragma once

#include <nlohmann/json.hpp>

class QString;

namespace shapecraft::impl {

template <typename T, std::size_t... I>
void to_json(nlohmann::json &json, const T &a, std::index_sequence<I...>) {
    json = {a[I]...};
}

template <typename T, std::size_t... I>
void from_json(const nlohmann::json &json, T &a, std::index_sequence<I...>) {
    a = {json[I]...};
}

} // namespace shapecraft::impl

void to_json(nlohmann::json &json, const QString &string);
void from_json(const nlohmann::json &json, QString &string);
