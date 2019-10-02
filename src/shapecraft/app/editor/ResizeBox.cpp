#include "ResizeBox.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include "shapecraft/util/Debug.hpp"
#include <QMouseEvent>
#include <array>

namespace shapecraft {

ResizeBoxEdge::ResizeBoxEdge(int axis, glm::ivec2 alignment) : _axis(axis), _alignment(alignment) {
}

void ResizeBoxEdge::draw(const DrawEvent &event) {
    updateVAO();
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, isHovered() ? 1.5 : 1, glm::vec4(0, 0, 1, 1));
}

void ResizeBoxEdge::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, 6, hitColor.toColor());
}

void ResizeBoxEdge::setPositions(const std::array<glm::dvec3, 2> &positions) {
    if (_positions == positions) {
        return;
    }

    _positions = positions;
    _isVAODirty = true;
    emit updated();
}

void ResizeBoxEdge::updateVAO() {
    if (!_isVAODirty) {
        return;
    }
    _isVAODirty = false;

    glm::dvec3 beginPosRatio;
    beginPosRatio[_axis] = 0;
    beginPosRatio[(_axis + 1) % 3] = _alignment.x;
    beginPosRatio[(_axis + 2) % 3] = _alignment.y;

    auto endPosRatio = beginPosRatio;
    endPosRatio[_axis] = 1;

    auto beginPos = glm::mix(_positions[0], _positions[1], beginPosRatio);
    auto endPos = glm::mix(_positions[0], _positions[1], endPosRatio);

    std::vector<draw::PointLineVertex> vertices{
        {beginPos, glm::vec4(0), 1},
        {endPos, glm::vec4(0), 1},
    };

    auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(vertices);
    _vao = std::make_shared<gl::VertexArray>(vbo, gl::Primitive::Line);
}

ResizeBoxVertex::ResizeBoxVertex(glm::dvec3 alignment) : _alignment(alignment) {
}

void ResizeBoxVertex::draw(const DrawEvent &event) {
    updateVAO();
    event.drawMethods->drawCircle(_vao, glm::mat4(1), event.camera, isHovered() ? 6 : 4, glm::vec4(0, 0, 1, 1));
}

void ResizeBoxVertex::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawCircle(_vao, glm::mat4(1), event.camera, 12, hitColor.toColor());
}

void ResizeBoxVertex::mousePressEvent(const MouseEvent &event) {
    glm::dvec3 worldPos = event.worldPos();

    _dragged = true;
    _dragInitPositions = _positions;
    _dragInitWorldPos = worldPos;
    _dragStarted = false;
}

void ResizeBoxVertex::mouseMoveEvent(const MouseEvent &event) {
    if (!_dragged) {
        return;
    }
    auto worldPos = event.worldPos();

    std::array<glm::dvec3, 2> positions = _dragInitPositions;
    positions[int(_alignment.x)].x = worldPos.x;
    positions[int(_alignment.y)].y = worldPos.y;
    positions[int(_alignment.z)].z = worldPos.z;

    emit positionsEdited(positions);
}

void ResizeBoxVertex::mouseReleaseEvent(const MouseEvent &event) {
    Q_UNUSED(event)
    _dragged = false;
}

void ResizeBoxVertex::setPositions(const std::array<glm::dvec3, 2> &positions) {
    if (_positions == positions) {
        return;
    }

    _positions = positions;
    _isVAODirty = true;
    emit updated();
}

void ResizeBoxVertex::updateVAO() {
    if (!_isVAODirty) {
        return;
    }
    _isVAODirty = false;

    auto position = glm::mix(_positions[0], _positions[1], _alignment);
    std::vector<draw::PointLineVertex> vertices{{position, glm::vec4(0), 1}};

    auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(vertices);
    _vao = std::make_shared<gl::VertexArray>(vbo, gl::Primitive::Point);
}

ResizeBox::ResizeBox() {
    std::vector<SP<Renderable>> children;

    std::vector<glm::dvec3> alignments = {
        glm::dvec3(0, 0, 0),
        glm::dvec3(0, 0, 1),
        glm::dvec3(0, 1, 0),
        glm::dvec3(0, 1, 1),
        glm::dvec3(1, 0, 0),
        glm::dvec3(1, 0, 1),
        glm::dvec3(1, 1, 0),
        glm::dvec3(1, 1, 1),
        glm::dvec3(0, 0.5, 0.5),
        glm::dvec3(1, 0.5, 0.5),
        glm::dvec3(0.5, 0, 0.5),
        glm::dvec3(0.5, 1, 0.5),
        glm::dvec3(0.5, 0.5, 0),
        glm::dvec3(0.5, 0.5, 1),
    };

    std::vector<std::pair<int, glm::ivec2>> edgeAlignments = {
        {0, {0, 0}},
        {0, {0, 1}},
        {0, {1, 0}},
        {0, {1, 1}},
        {1, {0, 0}},
        {1, {0, 1}},
        {1, {1, 0}},
        {1, {1, 1}},
        {2, {0, 0}},
        {2, {0, 1}},
        {2, {1, 0}},
        {2, {1, 1}},
    };

    for (auto &&a : alignments) {
        auto vertex = std::make_shared<ResizeBoxVertex>(a);
        connect(vertex.get(), &ResizeBoxVertex::positionsEdited, this, &ResizeBox::positionsEdited);
        _vertices.push_back(vertex);
        children.push_back(vertex);
    }

    for (auto &&[axis, alignment] : edgeAlignments) {
        auto edge = std::make_shared<ResizeBoxEdge>(axis, alignment);
        connect(edge.get(), &ResizeBoxEdge::positionsEdited, this, &ResizeBox::positionsEdited);
        _edges.push_back(edge);
        children.push_back(edge);
    }

    setChildRenderables(children);
}

void ResizeBox::setPositions(const std::array<glm::dvec3, 2> &positions) {
    if (_positions == positions) {
        return;
    }
    _positions = positions;

    for (auto &&vertex : _vertices) {
        vertex->setPositions(positions);
    }
    for (auto &&edge : _edges) {
        edge->setPositions(positions);
    }
}

} // namespace shapecraft
