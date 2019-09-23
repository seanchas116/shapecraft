#include "ResizeBox.hpp"

namespace shapecraft {

ResizeBox::ResizeBox() {
}

void ResizeBox::setBox(const Box<double> &box) {
    if (_box == box) {
        return;
    }

    _box = box;
    // TODO
}

} // namespace shapecraft
