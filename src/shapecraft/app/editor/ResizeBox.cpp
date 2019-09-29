#include "ResizeBox.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include "shapecraft/util/Debug.hpp"
#include <array>

namespace shapecraft {

ResizeBox::ResizeBox() {
}

void ResizeBox::setBox(const Box<double> &box) {
    if (_box == box) {
        return;
    }

    _box = box;
    _isVAODirty = true;
    emit updated();
}

void ResizeBox::draw(const DrawEvent &event) {
    updateVAO();
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, _isHovered ? 2 : 1, glm::vec4(0, 0, 1, 1));
}

void ResizeBox::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, 4, hitColor.toColor());
}

void ResizeBox::hoverEnterEvent(const MouseEvent &event) {
    Q_UNUSED(event)
    _isHovered = true;
    emit updated();
}

void ResizeBox::hoverLeaveEvent() {
    _isHovered = false;
    emit updated();
}

void ResizeBox::updateVAO() {
    if (!_isVAODirty) {
        return;
    }

    auto minPos = _box.minPosition();
    auto maxPos = _box.maxPosition();

    std::vector<glm::vec3> positions = {
        glm::vec3(minPos.x, minPos.y, minPos.z),
        glm::vec3(maxPos.x, minPos.y, minPos.z),
        glm::vec3(minPos.x, maxPos.y, minPos.z),
        glm::vec3(maxPos.x, maxPos.y, minPos.z),
        glm::vec3(minPos.x, minPos.y, maxPos.z),
        glm::vec3(maxPos.x, minPos.y, maxPos.z),
        glm::vec3(minPos.x, maxPos.y, maxPos.z),
        glm::vec3(maxPos.x, maxPos.y, maxPos.z),
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
    _vao = std::make_shared<gl::VertexArray>(vbo, ibo);

    _isVAODirty = false;
}

} // namespace shapecraft
