#pragma once

#include "shapecraft/render/viewport/Renderable.hpp"
#include "shapecraft/util/Box.hpp"
#include <QObject>

namespace shapecraft {

class ResizeBoxVertex : public viewport::Renderable {
    Q_OBJECT
  public:
    ResizeBoxVertex(glm::dvec3 alignment);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void hoverEnterEvent(const MouseEvent &event) override;
    void hoverLeaveEvent() override;

    void setBox(const Box<double> &box);

  private:
    void updateVAO();

    Box<double> _box;
    glm::dvec3 _alignment;
    bool _isVAODirty = true;
    SP<gl::VertexArray> _vao;
    bool _isHovered = false;
};

class ResizeBox : public viewport::Renderable {
    Q_OBJECT
  public:
    ResizeBox();

    void setBox(const Box<double> &box);

    void draw(const DrawEvent &event) override;
    void drawHitArea(const DrawEvent &event, const viewport::HitColor &hitColor) override;

    void hoverEnterEvent(const MouseEvent &event) override;
    void hoverLeaveEvent() override;

  private:
    void updateVAOs();

    Box<double> _box;
    std::vector<SP<ResizeBoxVertex>> _vertices;

    bool _isVAOsDirty = true;
    SP<gl::VertexArray> _edgesVAO;
    SP<gl::VertexArray> _cornersVAO;

    bool _isHovered = false;
};

} // namespace shapecraft
