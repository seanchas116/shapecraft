#pragma once

#include "OpenGLEntity.hpp"
#include <glm/vec2.hpp>

namespace shapecraft {
namespace gl {

class Texture final : public OpenGLEntity {
  public:
    enum class Format {
        RGBA8,
        RGBA32F,
        Depth24Stencil8
    };

    Texture(glm::ivec2 size, Format format = Format::RGBA8, const void *pixels = nullptr);
    ~Texture();

    glm::ivec2 size() const { return _size; }
    GLuint name() const { return _name; }

    void bind();
    void unbind();

  private:
    GLuint _name = 0;
    glm::ivec2 _size;
};

} // namespace gl
} // namespace shapecraft
