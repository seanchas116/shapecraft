#pragma once
#include "shapecraft/Common.hpp"
#include "shapecraft/render/draw/DrawMethods.hpp"
#include <QOpenGLWidget>

namespace shapecraft {
namespace viewport {

class Viewport;
class Renderable;

class ViewportContainer : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT
  public:
    ViewportContainer(QWidget *parent = nullptr);

    auto &&layers() const { return _layers; }
    void setLayers(const std::vector<SP<Renderable>> &layers);

  signals:
    void resized();
    void initialized();
    void aboutToBePainted();

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    std::vector<SP<Renderable>> _layers;
    Opt<SP<draw::DrawMethods>> _drawMethods;
};

} // namespace viewport
} // namespace shapecraft
