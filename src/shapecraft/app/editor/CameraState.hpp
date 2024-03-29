#pragma once
#include "shapecraft/util/Camera.hpp"
#include "shapecraft/util/Property.hpp"
#include <QObject>
#include <glm/glm.hpp>

namespace shapecraft {

class CameraState : public QObject {
    Q_OBJECT

  public:
    enum class Orientation {
        None,
        Front,
        Back,
        Right,
        Left,
        Top,
        Bottom,
    };

    CameraState();

    SHAPECRAFT_PROPERTY(Camera::Projection, projection, setProjection, Camera::Projection::Perspective)
    SHAPECRAFT_PROPERTY(glm::dvec3, position, setPosition, glm::dvec3(0, 10, 10))
    SHAPECRAFT_PROPERTY(glm::dvec3, eulerAngles, setEulerAngles, glm::dvec3(0))
    SHAPECRAFT_PROPERTY(glm::dvec2, viewportSize, setViewportSize, glm::dvec2(100, 100))
    SHAPECRAFT_PROPERTY(double, fieldOfView, setFieldOfView, glm::radians(60.0))
    SHAPECRAFT_PROPERTY(double, zNear, setZNear, 0.1)
    SHAPECRAFT_PROPERTY(double, zFar, setZFar, 100.0)
    SHAPECRAFT_PROPERTY(double, orthoScale, setOrthoScale, 100.0)

    Camera camera() const;

    glm::dmat4 cameraToWorldMatrix() const;

    Orientation orientation() const;
    void setOrientation(Orientation orientation);

  signals:
    void cameraChanged(const Camera &camera);
    void orientationChanged(Orientation orientation);

  private:
    static glm::dvec3 orientationAngle(Orientation orientation);

    void emitCameraChanged();
};

} // namespace shapecraft
