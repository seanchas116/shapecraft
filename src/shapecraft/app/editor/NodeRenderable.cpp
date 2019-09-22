#include "NodeRenderable.hpp"
#include "shapecraft/document/ShapeNode.hpp"
#include "shapecraft/render/draw/Vertex.hpp"
#include "shapecraft/render/gl/VertexArray.hpp"
#include "shapecraft/render/gl/VertexBuffer.hpp"
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Poly.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

namespace shapecraft {

NodeRenderable::NodeRenderable(const SP<Node> &node) {
    if (auto shapeNode = std::dynamic_pointer_cast<ShapeNode>(node); shapeNode) {
        setShape(shapeNode->shape());
    }
}

void NodeRenderable::draw(const viewport::DrawEvent &event) {
    draw::Material material;
    material.baseColor = glm::vec3(1);
    event.operations->drawMaterial.draw(_facesVAO, glm::mat4(1), event.camera, material);
    event.operations->drawLine.draw(_edgesVAO, glm::mat4(1), event.camera, 1, glm::vec4(0, 0, 0, 1));
}

void NodeRenderable::setShape(const TopoDS_Shape &shape) {
    BRepMesh_IncrementalMesh meshing(shape, 0.01, false, 0.5);
    meshing.Perform();

    QHash<Poly_Triangulation *, int> indexOffsets;

    std::vector<draw::Vertex> vertices;
    std::vector<draw::PointLineVertex> edgeVertices;
    {
        TopExp_Explorer explorer(shape, TopAbs_FACE);

        std::vector<std::array<uint32_t, 3>> triangleIndexes;

        for (; explorer.More(); explorer.Next()) {
            TopoDS_Face face = TopoDS::Face(explorer.Current());
            TopLoc_Location location;
            auto triangulation = BRep_Tool::Triangulation(face, location);

            const TColgp_Array1OfPnt &nodes = triangulation->Nodes();

            int indexOffset = int(vertices.size()) - nodes.Lower();
            indexOffsets.insert(triangulation.get(), indexOffset);

            for (int i = 0; i < nodes.Length(); ++i) {
                gp_Pnt p = nodes.Value(nodes.Lower() + i);
                glm::vec3 pos(p.X(), p.Y(), p.Z());

                gp_Pnt2d uv = triangulation->UVNodes()(triangulation->UVNodes().Lower() + i);

                TopLoc_Location location;
                auto surface = BRep_Tool::Surface(face, location);
                GeomLProp_SLProps props(surface, uv.X(), uv.Y(), 1, 0.01);
                glm::vec3 n(0);
                if (props.IsNormalDefined()) {
                    auto normal = props.Normal();
                    n = glm::vec3(normal.X(), normal.Y(), normal.Z());
                    if (face.Orientation() == TopAbs_REVERSED) {
                        n = -n;
                    }
                }
                vertices.push_back({pos, {0, 0}, n});
                edgeVertices.push_back({pos, {0, 0, 0, 0}, 1});
            }
            const Poly_Array1OfTriangle &triangles = triangulation->Triangles();
            for (int i = triangles.Lower(); i <= triangles.Upper(); ++i) {
                Poly_Triangle t = triangles.Value(i);
                auto t1 = uint32_t(t.Value(1) + indexOffset);
                auto t2 = uint32_t(t.Value(2) + indexOffset);
                auto t3 = uint32_t(t.Value(3) + indexOffset);
                triangleIndexes.push_back({{t1, t2, t3}});
            }
        }

        auto vbo = std::make_shared<gl::VertexBuffer<draw::Vertex>>(vertices);
        auto ibo = std::make_shared<gl::IndexBuffer>(triangleIndexes);
        _facesVAO = std::make_shared<gl::VertexArray>(vbo, ibo);
    }

    {
        std::vector<std::array<uint32_t, 2>> lines;

        TopExp_Explorer edgeExplorer(shape, TopAbs_EDGE);
        for (; edgeExplorer.More(); edgeExplorer.Next()) {
            auto edge = TopoDS::Edge(edgeExplorer.Current());
            Handle(Poly_PolygonOnTriangulation) polygon;
            Handle(Poly_Triangulation) triangulation;
            TopLoc_Location location;
            BRep_Tool::PolygonOnTriangulation(edge, polygon, triangulation, location);

            int indexOffset = indexOffsets[triangulation.get()];
            auto &indices = polygon->Nodes();
            for (int i = indices.Lower() + 1; i <= indices.Upper(); ++i) {
                auto begin = uint16_t(indices.Value(i - 1) + indexOffset);
                auto end = uint16_t(indices.Value(i) + indexOffset);
                lines.push_back({begin, end});
            }
        }

        auto vbo = std::make_shared<gl::VertexBuffer<draw::PointLineVertex>>(edgeVertices);
        auto ibo = std::make_shared<gl::IndexBuffer>(lines);
        _edgesVAO = std::make_shared<gl::VertexArray>(vbo, ibo);
    }
}

} // namespace shapecraft
