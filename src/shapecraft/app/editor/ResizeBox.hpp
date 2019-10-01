#pragma once

#include "shapecraft/render/viewport/HoverableRenderable.hpp"
#include "shapecraft/util/Box.hpp"
#include <QObject>

namespace shapecraft {

class ResizeBoxEdge : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBoxEdge(int axis, glm::ivec2 alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void setBox(const Box<double> &box);

  private:
    void updateVAO();

    Box<double> _box;
    int _axis;
    glm::ivec2 _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;
};

class ResizeBoxVertex : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBoxVertex(glm::dvec3 alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void setBox(const Box<double> &box);

  private:
    void updateVAO();

    Box<double> _box;
    glm::dvec3 _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;
};

class ResizeBox : public viewport::HoverableRenderable {
    Q_OBJECT
  public:
    ResizeBox();

    void setBox(const Box<double> &box);

  private:
    Box<double> _box;
    std::vector<SP<ResizeBoxVertex>> _vertices;
    std::vector<SP<ResizeBoxEdge>> _edges;
};

} // namespace shapecraft
