#include "ResizeBox.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include "shapecraft/util/Debug.hpp"
#include <array>

namespace shapecraft {

ResizeBox::ResizeBox() {
    updateVertexArray();
}

void ResizeBox::setBox(const Box<double> &box) {
    if (_box == box) {
        return;
    }

    _box = box;
    updateVertexArray();
}

void ResizeBox::draw(const DrawEvent &event) {
    qDebug() << _box;
    event.drawMethods->drawLine(_vertexArray, glm::mat4(1), event.camera, 1, glm::vec4(0, 0, 1, 1));
}

void ResizeBox::updateVertexArray() {
    std::vector<glm::vec3> positions = {
        glm::vec3(_box.minPosition.x, _box.minPosition.y, _box.minPosition.z),
        glm::vec3(_box.maxPosition.x, _box.minPosition.y, _box.minPosition.z),
        glm::vec3(_box.minPosition.x, _box.maxPosition.y, _box.minPosition.z),
        glm::vec3(_box.maxPosition.x, _box.maxPosition.y, _box.minPosition.z),
        glm::vec3(_box.minPosition.x, _box.minPosition.y, _box.maxPosition.z),
        glm::vec3(_box.maxPosition.x, _box.minPosition.y, _box.maxPosition.z),
        glm::vec3(_box.minPosition.x, _box.maxPosition.y, _box.maxPosition.z),
        glm::vec3(_box.maxPosition.x, _box.maxPosition.y, _box.maxPosition.z),
    };

    std::vector<std::array<uint32_t, 2>> lines = {
        {0, 4},
        {4, 6},
        {6, 2},
        {2, 0},
        {0, 1},
        {4, 5},
        {6, 7},
        {2, 3},
        {1, 5},
        {5, 7},
        {7, 3},
        {3, 1},
    };

    std::vector<draw::PointLineVertex> vertices;

    for (auto &&pos : positions) {
        vertices.push_back({pos, glm::vec4(0), 1});
    }

    auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(vertices);
    auto ibo = std::make_shared<gl::IndexBuffer>(lines);
    _vertexArray = std::make_shared<gl::VertexArray>(vbo, ibo);
}

} // namespace shapecraft
