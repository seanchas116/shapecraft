#pragma once
#include "lattice/Common.hpp"
#include "lattice/render/draw/Operations.hpp"
#include <QOpenGLWidget>

namespace lattice {
namespace Viewport {

class Viewport;

class ViewportContainer : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT
  public:
    ViewportContainer(QWidget *parent = nullptr);

    auto &viewports() const { return _viewports; }
    void setViewports(const std::vector<Viewport *> &viewports);

  signals:
    void resized();
    void initialized();
    void aboutToBePainted();

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    std::vector<Viewport *> _viewports;
    Opt<SP<draw::Operations>> _operations;
};

} // namespace Viewport
} // namespace lattice
