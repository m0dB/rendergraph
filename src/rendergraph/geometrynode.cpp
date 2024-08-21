#include "rendergraph/geometrynode.h"

#include "geometrynode_impl.h"
#include "rendergraph/geometry.h"

using namespace rendergraph;

GeometryNode::GeometryNode(GeometryNode::Impl* pImpl)
        : Node(pImpl) {
}

GeometryNode::GeometryNode()
        : GeometryNode(new GeometryNode::Impl) {
}

GeometryNode::~GeometryNode() = default;

GeometryNode::Impl& GeometryNode::impl() const {
    return static_cast<GeometryNode::Impl&>(Node::impl());
}

void GeometryNode::setGeometry(std::unique_ptr<Geometry> geometry) {
    m_geometry = std::move(geometry);
    impl().setGeometry(m_geometry.get());
}

void GeometryNode::setMaterial(std::unique_ptr<Material> material) {
    m_material = std::move(material);
    impl().setMaterial(m_material.get());
}

Geometry& GeometryNode::geometry() const {
    return *m_geometry;
}

Material& GeometryNode::material() const {
    return *m_material;
}
