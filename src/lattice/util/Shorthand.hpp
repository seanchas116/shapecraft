#pragma once

#include <memory>
#include <optional>

namespace lattice {

template <class T>
using SP = std::shared_ptr<T>;

template <class T>
using WP = std::weak_ptr<T>;

template <class T>
using Opt = std::optional<T>;

} // namespace lattice
