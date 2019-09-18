#include "Resource.hpp"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(ShapecraftResources);

namespace shapecraft {
namespace resource {

std::string read(const std::string &path) {
    auto file = cmrc::ShapecraftResources::get_filesystem().open(path);
    return std::string(file.begin(), file.end());
}

} // namespace resource
} // namespace shapecraft
