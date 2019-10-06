#pragma once

#include "shapecraft/render/viewport/HoverableRenderable.hpp"
#include "shapecraft/util/Box.hpp"
#include <QObject>

namespace shapecraft {

class ResizeBoxFace : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBoxFace(int axis, int alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void mousePressEvent(const MouseEvent &event) override;
    void mouseMoveEvent(const MouseEvent &event) override;
    void mouseReleaseEvent(const MouseEvent &event) override;

    void setPositions(const std::array<glm::dvec3, 2> &positions);

  signals:
    void editStarted();
    void positionsEdited(const std::array<glm::dvec3, 2> &box);

  private:
    void updateVAO();

    std::array<glm::dvec3, 2> _positions;
    int _axis;
    int _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;

    bool _dragged = false;
    std::array<glm::dvec3, 2> _dragInitPositions;
    glm::dvec3 _dragInitWorldPos;
};

class ResizeBoxEdge : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBoxEdge(int axis, glm::ivec2 alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void mousePressEvent(const MouseEvent &event) override;
    void mouseMoveEvent(const MouseEvent &event) override;
    void mouseReleaseEvent(const MouseEvent &event) override;

    void setPositions(const std::array<glm::dvec3, 2> &positions);

  signals:
    void editStarted();
    void positionsEdited(const std::array<glm::dvec3, 2> &box);

  private:
    std::array<glm::dvec3, 2> edgePositions() const;
    void updateVAO();

    std::array<glm::dvec3, 2> _positions;
    int _axis;
    glm::ivec2 _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;

    bool _dragged = false;
    std::array<glm::dvec3, 2> _dragInitPositions;
    glm::dvec3 _dragInitWorldPos;
};

class ResizeBoxVertex : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBoxVertex(glm::dvec3 alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void mousePressEvent(const MouseEvent &event) override;
    void mouseMoveEvent(const MouseEvent &event) override;
    void mouseReleaseEvent(const MouseEvent &event) override;

    void setPositions(const std::array<glm::dvec3, 2> &positions);

  signals:
    void editStarted();
    void positionsEdited(const std::array<glm::dvec3, 2> &box);

  private:
    void updateVAO();

    std::array<glm::dvec3, 2> _positions;
    glm::dvec3 _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;

    bool _dragged = false;
    std::array<glm::dvec3, 2> _dragInitPositions;
    glm::dvec3 _dragInitWorldPos;
};

class ResizeBox : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBox();

    void setPositions(const std::array<glm::dvec3, 2> &positions);

  signals:
    void editStarted();
    void positionsEdited(const std::array<glm::dvec3, 2> &box);

  private:
    std::array<glm::dvec3, 2> _positions;
    std::vector<SP<ResizeBoxVertex>> _vertices;
    std::vector<SP<ResizeBoxEdge>> _edges;
    std::vector<SP<ResizeBoxFace>> _faces;
};

} // namespace shapecraft
