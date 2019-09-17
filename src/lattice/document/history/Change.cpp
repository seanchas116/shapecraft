#include "Change.hpp"

namespace lattice {

Change::~Change() = default;

bool Change::mergeWith(const SP<const Change> &other) {
    Q_UNUSED(other)
    return false;
}

} // namespace lattice
