#pragma once

#include <QOpenGLFunctions>

#include "rendergraph/geometrynode.h"

#include "material_impl.h"
#include "node_impl.h"

class rendergraph::GeometryNode::Impl : public rendergraph::Node::Impl, public QOpenGLFunctions {
  public:
    Impl() {
    }

    void setGeometry(Geometry* geometry) {
        m_pGeometry = geometry;
    }

    void setMaterial(Material* material) {
        m_pMaterial = material;
    }

    void initialize() override {
        initializeOpenGLFunctions();
        m_pMaterial->impl().setShader(m_pMaterial->createShader());
        Node::Impl::initialize();
    }

    void render() override;

  private:
    Geometry* m_pGeometry{};
    Material* m_pMaterial{};
};
