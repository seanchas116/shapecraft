#include "Resource.hpp"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(LatticeResources);

namespace lattice {
namespace resource {

std::string read(const std::string &path) {
    auto file = cmrc::LatticeResources::get_filesystem().open(path);
    return std::string(file.begin(), file.end());
}

} // namespace resource
} // namespace lattice
