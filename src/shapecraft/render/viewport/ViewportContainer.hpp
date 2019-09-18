#pragma once
#include "shapecraft/Common.hpp"
#include "shapecraft/render/draw/Operations.hpp"
#include <QOpenGLWidget>

namespace shapecraft {
namespace viewport {

class Viewport;

class ViewportContainer : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT
  public:
    ViewportContainer(QWidget *parent = nullptr);

  signals:
    void resized();
    void initialized();
    void aboutToBePainted();

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    Opt<SP<draw::Operations>> _operations;
};

} // namespace viewport
} // namespace shapecraft
