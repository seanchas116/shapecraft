#include "ResizeBox.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include "shapecraft/util/Debug.hpp"
#include "shapecraft/util/Distance.hpp"
#include <QMouseEvent>
#include <array>
#include <glm/gtx/transform.hpp>

namespace shapecraft {

ResizeBoxFace::ResizeBoxFace(int axis, int alignment) : _axis(axis), _alignment(alignment) {
}

void ResizeBoxFace::draw(const viewport::Renderable::DrawEvent &event) {
    Q_UNUSED(event)
    updateVAO();
    glm::vec4 color(0, 0, 0, 1);
    color[_axis] = 1;
    event.drawMethods->drawUnicolor(_vao, glm::mat4(1), event.camera, color);
}

void ResizeBoxFace::drawHitArea(const viewport::Renderable::DrawEvent &event, const viewport::HitColor &hitColor) {
    Q_UNUSED(event)
    Q_UNUSED(hitColor)
}

void ResizeBoxFace::mousePressEvent(const viewport::Renderable::MouseEvent &event) {
    Q_UNUSED(event)
}

void ResizeBoxFace::mouseMoveEvent(const viewport::Renderable::MouseEvent &event) {
    Q_UNUSED(event)
}

void ResizeBoxFace::mouseReleaseEvent(const viewport::Renderable::MouseEvent &event) {
    Q_UNUSED(event)
}

void ResizeBoxFace::setPositions(const std::array<glm::dvec3, 2> &positions) {
    if (_positions == positions) {
        return;
    }

    _positions = positions;
    _isVAODirty = true;
    emit updated();
}

void ResizeBoxFace::updateVAO() {
    if (!_isVAODirty) {
        return;
    }
    _isVAODirty = false;

    std::vector<draw::Vertex> vertices;
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 2; ++i) {
            glm::vec3 alignments;
            alignments[_axis] = _alignment;
            alignments[(_axis + 1) % 3] = i;
            alignments[(_axis + 2) % 3] = j;

            auto p = glm::mix(_positions[0], _positions[1], alignments);
            vertices.push_back({p});
        }
    }

    std::vector<gl::IndexBuffer::Triangle> triangles;
    if (_alignment == 0) {
        triangles.push_back({2, 1, 0});
        triangles.push_back({1, 2, 3});

    } else {
        triangles.push_back({0, 1, 2});
        triangles.push_back({3, 2, 1});
    }

    auto vbo = std::make_shared<gl::VertexBuffer<draw::Vertex>>(vertices);
    auto ibo = std::make_shared<gl::IndexBuffer>(triangles);
    _vao = std::make_shared<gl::VertexArray>(vbo, ibo);
}

ResizeBoxEdge::ResizeBoxEdge(int axis, glm::ivec2 alignment) : _axis(axis), _alignment(alignment) {
}

void ResizeBoxEdge::draw(const DrawEvent &event) {
    updateVAO();
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, isHovered() ? 1.5 : 1, glm::vec4(0, 0, 1, 1));
}

void ResizeBoxEdge::drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) {
    event.drawMethods->drawLine(_vao, glm::mat4(1), event.camera, 6, hitColor.toColor());
}

void ResizeBoxEdge::mousePressEvent(const viewport::Renderable::MouseEvent &event) {
    glm::dvec3 worldPos = event.worldPos();

    _dragged = true;
    _dragInitPositions = _positions;
    _dragInitWorldPos = worldPos;

    emit editStarted();
}

void ResizeBoxEdge::mouseMoveEvent(const viewport::Renderable::MouseEvent &event) {
    if (!_dragged) {
        return;
    }

    auto edgeBeginPos = edgePositions()[0];
    std::array<glm::dvec3, 2> positions = _dragInitPositions;

    // TODO: Make code more self descriptive

    for (auto &&planeNormal : event.camera.mostFacingPlaneNormals()) {
        if (planeNormal == _axis) {
            continue;
        }

        auto worldMouseRay = event.camera.worldMouseRay(event.viewportPos);
        auto dragSpaceRay = glm::translate(-edgeBeginPos) * worldMouseRay;

        int axis0 = (_axis + 1) % 3;
        int axis1 = (_axis + 2) % 3;
        if (axis0 == planeNormal) {
            auto newPos = dragSpaceRay.planeIntercept(axis0) + edgeBeginPos;
            positions[_alignment[1]][axis1] = newPos[axis1];
        } else {
            auto newPos = dragSpaceRay.planeIntercept(axis1) + edgeBeginPos;
            positions[_alignment[0]][axis0] = newPos[axis0];
        }
        break;
    }

    /*
    for (int i = 0; i < 2; ++i) {
        int axis = (_axis + i + 1) % 3;
        positions[_alignment[i]][axis] = worldPos[axis];
    }
    */

    emit positionsEdited(positions);
}

void ResizeBoxEdge::mouseReleaseEvent(const viewport::Renderable::MouseEvent &event) {
    Q_UNUSED(event)
    _dragged = false;
}

void ResizeBoxEdge::setPositions(const std::array<glm::dvec3, 2> &positions) {
    if (_positions == positions) {
        return;
    }

    _positions = positions;
    _isVAODirty = true;
    emit updated();
}

std::array<glm::dvec3, 2> ResizeBoxEdge::edgePositions() const {
    glm::dvec3 beginPosRatio;
    beginPosRatio[_axis] = 0;
    beginPosRatio[(_axis + 1) % 3] = _alignment.x;
    beginPosRatio[(_axis + 2) % 3] = _alignment.y;

    auto endPosRatio = beginPosRatio;
    endPosRatio[_axis] = 1;

    auto beginPos = glm::mix(_positions[0], _positions[1], beginPosRatio);
    auto endPos = glm::mix(_positions[0], _positions[1], endPosRatio);

    return {beginPos, endPos};
}

void ResizeBoxEdge::updateVAO() {
    if (!_isVAODirty) {
        return;
    }
    _isVAODirty = false;

    auto segment = edgePositions();
    std::vector<draw::PointLineVertex> vertices{
        {segment[0], glm::vec4(0), 1},
        {segment[1], glm::vec4(0), 1},
    };

    auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(vertices);
    _vao = std::make_shared<gl::VertexArray>(vbo, gl::IndexBuffer::Primitive::Line);
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

    emit editStarted();
}

void ResizeBoxVertex::mouseMoveEvent(const MouseEvent &event) {
    if (!_dragged) {
        return;
    }
    auto handlePos = glm::mix(_dragInitPositions[0], _dragInitPositions[1], _alignment);

    std::array<glm::dvec3, 2> positions = _dragInitPositions;

    [&] {
        // face-center handles
        for (int i = 0; i < 3; ++i) {
            if (_alignment[(i + 1) % 3] == 0.5 && _alignment[(i + 2) % 3] == 0.5) {
                glm::dvec3 axisVec(0);
                axisVec[i] = 1;
                Ray<double> axisRay(handlePos, axisVec);

                positions[_alignment[i]][i] += event.camera.mapCameraToAxis(axisRay, event.viewportPos);
                return;
            }
        }

        // drag in facing plane
        auto facingPlaneNormal = event.camera.mostFacingPlaneNormals()[0];

        auto worldMouseRay = event.camera.worldMouseRay(event.viewportPos);
        auto dragSpaceRay = glm::translate(-handlePos) * worldMouseRay;
        auto newPos = dragSpaceRay.planeIntercept(facingPlaneNormal) + handlePos;

        for (int i = 0; i < 3; ++i) {
            positions[int(_alignment[i])][i] = newPos[i];
        }
    }();

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
    _vao = std::make_shared<gl::VertexArray>(vbo, gl::IndexBuffer::Primitive::Point);
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

    std::vector<std::pair<int, int>> faceAlignments = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
        {2, 0},
        {2, 1},
    };

    for (auto &&a : alignments) {
        auto vertex = std::make_shared<ResizeBoxVertex>(a);
        connect(vertex.get(), &ResizeBoxVertex::editStarted, this, &ResizeBox::editStarted);
        connect(vertex.get(), &ResizeBoxVertex::positionsEdited, this, &ResizeBox::positionsEdited);
        _vertices.push_back(vertex);
    }

    for (auto &&[axis, alignment] : edgeAlignments) {
        auto edge = std::make_shared<ResizeBoxEdge>(axis, alignment);
        connect(edge.get(), &ResizeBoxEdge::editStarted, this, &ResizeBox::editStarted);
        connect(edge.get(), &ResizeBoxEdge::positionsEdited, this, &ResizeBox::positionsEdited);
        _edges.push_back(edge);
    }

    for (auto &&[axis, alignment] : faceAlignments) {
        auto face = std::make_shared<ResizeBoxFace>(axis, alignment);
        connect(face.get(), &ResizeBoxFace::editStarted, this, &ResizeBox::editStarted);
        connect(face.get(), &ResizeBoxFace::positionsEdited, this, &ResizeBox::positionsEdited);
        _faces.push_back(face);
    }
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
    for (auto &&face : _faces) {
        face->setPositions(positions);
    }
}

void ResizeBox::setVisible(bool visible) {
    for (auto &&vertex : _vertices) {
        vertex->setVisible(visible);
    }
    for (auto &&edge : _edges) {
        edge->setVisible(visible);
    }
    for (auto &&face : _faces) {
        face->setVisible(visible);
    }
}

} // namespace shapecraft
