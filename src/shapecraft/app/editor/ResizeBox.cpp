#include "ResizeBox.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include "shapecraft/util/Debug.hpp"
#include <array>

namespace shapecraft {

ResizeBoxVertex::ResizeBoxVertex(glm::dvec3 alignment) : _alignment(alignment) {
}

void ResizeBoxVertex::draw(const viewport::Renderable::DrawEvent &event) {
    updateVAO();
    event.drawMethods->drawCircle(_vao, glm::mat4(1), event.camera, _isHovered ? 6 : 4, glm::vec4(0, 0, 1, 1));
}

void ResizeBoxVertex::drawHitArea(const viewport::Renderable::DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawCircle(_vao, glm::mat4(1), event.camera, _isHovered ? 6 : 4, hitColor.toColor());
}

void ResizeBoxVertex::hoverEnterEvent(const viewport::Renderable::MouseEvent &event) {
    Q_UNUSED(event)
    _isHovered = true;
    emit updated();
}

void ResizeBoxVertex::hoverLeaveEvent() {
    _isHovered = false;
    emit updated();
}

void ResizeBoxVertex::setBox(const Box<double> &box) {
    if (_box == box) {
        return;
    }

    _box = box;
    _isVAODirty = true;
    emit updated();
}

void ResizeBoxVertex::updateVAO() {
    if (!_isVAODirty) {
        return;
    }
    _isVAODirty = false;

    auto position = glm::mix(_box.minPosition(), _box.maxPosition(), _alignment);
    std::vector<draw::PointLineVertex> vertices{{position, glm::vec4(0), 1}};

    auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(vertices);
    _vao = std::make_shared<gl::VertexArray>(vbo, gl::Primitive::Point);
}

ResizeBox::ResizeBox() {
    std::vector<glm::dvec3> alignments = {
        glm::dvec3(0, 0, 0),
        glm::dvec3(0, 0, 1),
        glm::dvec3(0, 1, 0),
        glm::dvec3(0, 1, 1),
        glm::dvec3(1, 0, 0),
        glm::dvec3(1, 0, 1),
        glm::dvec3(1, 1, 0),
        glm::dvec3(1, 1, 1),
    };

    std::vector<SP<Renderable>> children;

    for (auto &&a : alignments) {
        auto vertex = std::make_shared<ResizeBoxVertex>(a);
        _vertices.push_back(vertex);
        children.push_back(vertex);
    }
    setChildRenderables(children);
}

void ResizeBox::setBox(const Box<double> &box) {
    if (_box == box) {
        return;
    }

    for (auto &&vertex : _vertices) {
        vertex->setBox(box);
    }

    _box = box;
    _isVAOsDirty = true;
    emit updated();
}

void ResizeBox::draw(const DrawEvent &event) {
    updateVAOs();
    event.drawMethods->drawLine(_edgesVAO, glm::mat4(1), event.camera, _isHovered ? 1.5 : 1, glm::vec4(0, 0, 1, 1));
}

void ResizeBox::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawLine(_edgesVAO, glm::mat4(1), event.camera, 4, hitColor.toColor());
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

void ResizeBox::updateVAOs() {
    if (!_isVAOsDirty) {
        return;
    }

    glm::vec3 minPos = _box.minPosition();
    glm::vec3 maxPos = _box.maxPosition();
    glm::vec3 midPos = (minPos + maxPos) * 0.5f;

    std::vector<glm::vec3> positions = {
        glm::vec3(minPos.x, minPos.y, minPos.z),
        glm::vec3(maxPos.x, minPos.y, minPos.z),
        glm::vec3(minPos.x, maxPos.y, minPos.z),
        glm::vec3(maxPos.x, maxPos.y, minPos.z),
        glm::vec3(minPos.x, minPos.y, maxPos.z),
        glm::vec3(maxPos.x, minPos.y, maxPos.z),
        glm::vec3(minPos.x, maxPos.y, maxPos.z),
        glm::vec3(maxPos.x, maxPos.y, maxPos.z),
        glm::vec3(minPos.x, midPos.yz),
        glm::vec3(maxPos.x, midPos.yz),
        glm::vec3(midPos.x, minPos.y, midPos.z),
        glm::vec3(midPos.x, maxPos.y, midPos.z),
        glm::vec3(midPos.xy, minPos.z),
        glm::vec3(midPos.xy, maxPos.z),
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
    _edgesVAO = std::make_shared<gl::VertexArray>(vbo, ibo);
    _cornersVAO = std::make_shared<gl::VertexArray>(vbo, gl::Primitive::Point);

    _isVAOsDirty = false;
}

} // namespace shapecraft
