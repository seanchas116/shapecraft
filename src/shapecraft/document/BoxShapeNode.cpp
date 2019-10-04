#include "BoxShapeNode.hpp"
#include "shapecraft/util/OCCConversion.hpp"
#include <BRepPrimAPI_MakeBox.hxx>
#include <QTimer>

namespace shapecraft {

BoxShapeNode::BoxShapeNode(const SP<History> &history) : ShapeNode(history) {
    connect(this, &ShapeNode::boundingBoxChanged, this, [this](const Box<double> &boundingBox) {
        if (boundingBox.size().x > 0 && boundingBox.size().y > 0 && boundingBox.size().z > 0) {
            auto shape = BRepPrimAPI_MakeBox(toOCC(boundingBox.minPosition()),
                                             toOCC(boundingBox.maxPosition()))
                             .Shape();
            setShape(shape);
        }
    });
}

const char *BoxShapeNode::type() const {
    return "boxShape";
}

SP<Node> BoxShapeNode::newInstance(const SP<History> &history) const {
    return std::make_shared<BoxShapeNode>(history);
}

} // namespace shapecraft
