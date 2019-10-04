#include "CylinderShapeNode.hpp"
#include "shapecraft/util/OCCConversion.hpp"
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <QTimer>
#include <glm/gtx/transform.hpp>

namespace shapecraft {

CylinderShapeNode::CylinderShapeNode(const SP<History> &history) : ShapeNode(history) {
    connect(this, &ShapeNode::boundingBoxChanged, this, [this](const Box<double> &boundingBox) {
        // TODO: create cylinder
        if (boundingBox.size().x > 0 && boundingBox.size().y > 0 && boundingBox.size().z > 0) {
            auto shape = BRepPrimAPI_MakeCylinder(0.5, 1).Shape();
            auto transform = glm::scale(boundingBox.size()) * glm::translate(glm::dvec3(0.5, 0.5, 0) + boundingBox.minPosition());
            setShape(BRepBuilderAPI_Transform(shape, toOCC(transform)).Shape());
        }
    });
}

const char *CylinderShapeNode::type() const {
    return "cylinderShapeNode";
}

SP<Node> CylinderShapeNode::newInstance(const SP<History> &history) const {
    return std::make_shared<CylinderShapeNode>(history);
}

} // namespace shapecraft
