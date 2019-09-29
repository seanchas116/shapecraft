#include "Demangle.hpp"
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <stdexcept>

namespace shapecraft {

std::string demangle(const char *mangledName) {
#ifdef _MSC_VER
    std::string mangledStr = mangledName;
    return mangledStr.substr(6); // Trim "class " (TODO: support non-class types)
#else
    int status;
    char *name = abi::__cxa_demangle(mangledName, 0, 0, &status);
    if (name == nullptr) {
        throw std::runtime_error("Cannot get class name");
    }
    std::string str = name;
    free(name);
    return str;
#endif
}

} // namespace shapecraft
