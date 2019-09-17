#pragma once

#include "lattice/Common.hpp"
#include <QDebug>
#include <QtDebug>
#include <array>
#include <optional>

namespace lattice {

template <typename T, size_t N>
QDebug operator<<(QDebug debug, const std::array<T, N> &array) {
    QDebugStateSaver saver(debug);
    debug.nospace();
    debug << "[";
    for (size_t i = 0; i < array.size(); ++i) {
        if (i != 0) {
            debug << ", ";
        }
        debug << array[i];
    }
    debug << "]";
    return debug;
}

template <typename T>
QDebug operator<<(QDebug debug, const Opt<T> &optional) {
    if (optional) {
        return debug << *optional;
    } else {
        return debug << "nullopt";
    }
}

template <typename T>
QDebug operator<<(QDebug debug, const SP<T> &ptr) {
    QDebugStateSaver saver(debug);
    debug.nospace();
    debug << "SP(";
    debug << ptr.get();
    debug << ")";
    return debug;
}

inline QDebug operator<<(QDebug debug, const std::string &str) {
    debug << QString::fromStdString(str);
    return debug;
}

} // namespace lattice
