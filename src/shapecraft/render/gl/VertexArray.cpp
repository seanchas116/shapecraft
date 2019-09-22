#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include <array>
#include <glm/glm.hpp>

namespace shapecraft {
namespace gl {

VertexArray::VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, const Opt<SP<IndexBuffer>> &indexBuffer, Primitive primitive) : _buffers(buffers),
                                                                                                                                                 _indexBuffer(indexBuffer),
                                                                                                                                                 _primitive(primitive) {
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);

    size_t indexOffset = 0;
    for (auto &[buffer, type] : buffers) {
        buffer->bind();
        auto attributes = buffer->attributes();
        GLsizei stride = 0;
        for (auto &a : attributes) {
            stride += a.sizePerComponent * a.count;
        }
        GLsizei offset = 0;

        for (size_t i = 0; i < attributes.size(); ++i) {
            auto &attribute = attributes[i];
            auto attribIndex = GLuint(indexOffset + i);
            glVertexAttribPointer(attribIndex, attribute.count, attribute.type, GL_FALSE, stride, reinterpret_cast<void *>(offset));
            offset += attribute.sizePerComponent * attribute.count;
            glEnableVertexAttribArray(attribIndex);
            if (type == BufferType::PerInstance) {
                glVertexAttribDivisor(attribIndex, 1);
            }
        }
        indexOffset += attributes.size();
    }

    if (indexBuffer) {
        indexBuffer->get()->bind();
    }

    glBindVertexArray(0);

    for (auto &[buffer, type] : buffers) {
        buffer->unbind();
    }
    if (indexBuffer) {
        indexBuffer->get()->unbind();
    }
}

struct EmptyVertex {};

VertexArray::VertexArray() : VertexArray(std::make_shared<VertexBuffer<EmptyVertex>>(), Primitive::Triangle) {
}

VertexArray::VertexArray(const SP<AnyVertexBuffer> &buffer, const SP<IndexBuffer> &indexBuffer) : VertexArray({{buffer, BufferType::PerVertex}}, indexBuffer) {
}

VertexArray::VertexArray(const SP<AnyVertexBuffer> &buffer, Primitive primitive) : VertexArray({{buffer, BufferType::PerVertex}}, primitive) {
}

VertexArray::VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, const SP<IndexBuffer> &indexBuffer) : VertexArray(buffers, indexBuffer, Primitive::Triangle) {
}

VertexArray::VertexArray(const std::vector<std::pair<SP<AnyVertexBuffer>, BufferType>> &buffers, Primitive primitive) : VertexArray(buffers, std::nullopt, primitive) {
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_vertexArray);
}

void VertexArray::draw() {
    glBindVertexArray(_vertexArray);

    if (_indexBuffer) {
        auto indexCount = GLsizei(_indexBuffer->get()->size());

        switch (_indexBuffer->get()->primitive()) {
        case Primitive::Point:
            glDrawElements(GL_POINTS, indexCount, GL_UNSIGNED_INT, nullptr);
            break;
        case Primitive::Line:
            glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
            break;
        case Primitive::Triangle:
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
            break;
        case Primitive::TriangleFan:
            glDrawElements(GL_TRIANGLE_FAN, indexCount, GL_UNSIGNED_INT, nullptr);
            break;
        }
    } else {
        auto bufferSize = GLsizei(_buffers[0].first->size());

        switch (_primitive) {
        case Primitive::Point:
            glDrawArrays(GL_POINTS, 0, bufferSize);
            break;
        case Primitive::Line:
            glDrawArrays(GL_LINES, 0, bufferSize);
            break;
        case Primitive::Triangle:
            glDrawArrays(GL_TRIANGLES, 0, bufferSize);
            break;
        case Primitive::TriangleFan:
            glDrawArrays(GL_TRIANGLE_FAN, 0, bufferSize);
            break;
        }
    }

    glBindVertexArray(0);
}

} // namespace gl
} // namespace shapecraft
