#pragma once
#include <QtGlobal>

namespace shapecraft {
namespace gl {

template <typename T>
class Binder final {
    Q_DISABLE_COPY(Binder)
  public:
    Binder(T &resource) : _resource(resource) {
        _resource.bind();
    }
    ~Binder() {
        _resource.unbind();
    }

  private:
    T &_resource;
};

} // namespace gl
} // namespace shapecraft
