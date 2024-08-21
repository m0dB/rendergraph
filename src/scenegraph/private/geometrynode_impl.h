#pragma once

#include "rendergraph/geometrynode.h"

#include "geometry_impl.h"
#include "material_impl.h"
#include "node_impl.h"

class rendergraph::GeometryNode::Impl : public rendergraph::Node::Impl {
  public:
    Impl()
            : Node::Impl(new QSGGeometryNode()) {
    }
    QSGGeometryNode* sgGeometryNode() {
        return static_cast<QSGGeometryNode*>(sgNode());
    }
    void setGeometry(Geometry* geometry) {
        sgGeometryNode()->setGeometry(geometry->impl().sgGeometry());
    }
    void setMaterial(Material* material) {
        sgGeometryNode()->setMaterial(material->impl().sgMaterial());
    }
};
