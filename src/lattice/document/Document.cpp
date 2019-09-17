#include "Document.hpp"
#include "history/History.hpp"

namespace lattice {

Document::Document() : Node(std::make_shared<History>()) {
}

} // namespace lattice
