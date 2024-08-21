#pragma once

#include "node_impl.h"
#include "material_impl.h"
#include "rendergraph/geometrynode.h"
#include <QOpenGLFunctions>

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

