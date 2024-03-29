#include "ViewportContainer.hpp"
#include "HitAreaMap.hpp"
#include "Renderable.hpp"
#include "Util.hpp"
#include "Viewport.hpp"
#include "shapecraft/util/Debug.hpp"
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QTransform>

namespace shapecraft {
namespace viewport {

ViewportContainer::ViewportContainer(QWidget *parent) : QOpenGLWidget(parent) {
}

void ViewportContainer::setLayers(const std::vector<SP<Renderable>> &layers) {
    for (auto &&layer : _layers) {
        disconnect(layer.get(), &Renderable::updated, this, qOverload<>(&ViewportContainer::update));
    }
    for (auto &&layer : layers) {
        connect(layer.get(), &Renderable::updated, this, qOverload<>(&ViewportContainer::update));
    }

    _layers = layers;
    update();
}

void ViewportContainer::initializeGL() {
    initializeOpenGLFunctions();

    auto logger = new QOpenGLDebugLogger(this);
    if (logger->initialize()) {
        connect(logger, &QOpenGLDebugLogger::messageLogged, [](const QOpenGLDebugMessage &message) {
            if (message.severity() == QOpenGLDebugMessage::NotificationSeverity) {
                return;
            }
            qWarning() << message.message();
        });
        logger->startLogging();
        qDebug() << "OpenGL debug enabled";
    }

    _drawMethods = std::make_shared<draw::DrawMethods>();
    emit initialized();
}

void ViewportContainer::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
    emit resized();
}

void ViewportContainer::paintGL() {
    emit aboutToBePainted();

    auto operations = *_drawMethods;

    auto viewports = findChildren<Viewport *>();

    for (auto viewport : viewports) {
        connect(viewport, &Viewport::updateRequested, this, [this] { update(); });
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (auto viewport : viewports) {
        Renderable::DrawEvent drawEvent{viewport, viewport->camera(), operations};

        glm::dvec2 minPos = mapQtToGL(this, viewport->mapTo(this, viewport->rect().bottomLeft() + QPoint(0, 1))); // right and bottom is 1px inset in integer QRect
        glm::dvec2 maxPos = mapQtToGL(this, viewport->mapTo(this, viewport->rect().topRight() + QPoint(1, 0)));
        glm::ivec2 minPosViewport = round(minPos * devicePixelRatioF());
        glm::ivec2 maxPosViewport = round(maxPos * devicePixelRatioF());
        glm::ivec2 sizeViewport = maxPosViewport - minPosViewport;

        glEnable(GL_SCISSOR_TEST);
        glScissor(minPosViewport.x, minPosViewport.y, sizeViewport.x, sizeViewport.y);
        glViewport(minPosViewport.x, minPosViewport.y, sizeViewport.x, sizeViewport.y);
        glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

        for (auto &&layer : _layers) {
            operations->clearDepth(1);
            layer->drawRecursive(drawEvent);
        }

        glDisable(GL_SCISSOR_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto &hitAreaMaps = viewport->hitAreaMaps(_layers.size());
        for (size_t i = 0; i < _layers.size(); ++i) {
            hitAreaMaps[i]->draw(_layers[i], drawEvent);
        }
    }

    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    QOpenGLPaintDevice device(width() * devicePixelRatioF(), height() * devicePixelRatioF());
    QPainter painter(&device);
    painter.setRenderHint(QPainter::Antialiasing);

    for (auto viewport : viewports) {
        painter.save();
        auto offset = viewport->mapTo(this, viewport->rect().topLeft());
        auto transform = QTransform(1, 0, 0, -1, 0, viewport->rect().height()) * QTransform::fromTranslate(offset.x(), offset.y()) * QTransform::fromScale(devicePixelRatioF(), devicePixelRatioF());
        painter.setTransform(transform);
        painter.setClipRect(0, 0, viewport->width(), viewport->height());

        Renderable::Draw2DEvent event{viewport, viewport->size(), &painter};

        for (auto &&layer : _layers) {
            layer->draw2DRecursive(event);
        }

        painter.restore();
    }
}

} // namespace viewport
} // namespace shapecraft
