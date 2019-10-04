#include "CylinderShapeNode.hpp"
#include <BRepPrimAPI_MakeBox.hxx>
#include <QTimer>

namespace shapecraft {

CylinderShapeNode::CylinderShapeNode(const SP<History> &history) : ShapeNode(history) {
    connect(this, &ShapeNode::boundingBoxChanged, this, [this](const Box<double> &boundingBox) {
        // TODO: create cylinder
        if (boundingBox.size().x > 0 && boundingBox.size().y > 0 && boundingBox.size().z > 0) {
            gp_Pnt p0(boundingBox.minPosition().x, boundingBox.minPosition().y, boundingBox.minPosition().z);
            gp_Pnt p1(boundingBox.maxPosition().x, boundingBox.maxPosition().y, boundingBox.maxPosition().z);
            setShape(BRepPrimAPI_MakeBox(p0, p1).Shape());
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
