#include "Change.hpp"

namespace shapecraft {

Change::~Change() = default;

bool Change::mergeWith(const SP<const Change> &other) {
    Q_UNUSED(other)
    return false;
}

} // namespace shapecraft
