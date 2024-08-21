#include "geometrynode_impl.h"

#include "geometry_impl.h"

using namespace rendergraph;

namespace {
GLenum toGlDrawingMode(Geometry::DrawingMode mode) {
    switch (mode) {
    case Geometry::DrawingMode::Triangles:
        return GL_TRIANGLES;
    case Geometry::DrawingMode::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    }
}
} // namespace

void GeometryNode::Impl::render() {
    Material::Impl& material = m_pMaterial->impl();
    Geometry::Impl& geometry = m_pGeometry->impl();

    QOpenGLShaderProgram& shader = material.glShader();
    shader.bind();

    if (m_pMaterial->clearUniformsCacheDirty()) {
        const UniformsCache& cache = m_pMaterial->uniformsCache();
        for (int i = 0; i < cache.count(); i++) {
            int location = material.uniformLocation(i);
            switch (cache.type(i)) {
            case Type::UInt:
                shader.setUniformValue(location, cache.get<GLuint>(i));
                break;
            case Type::Float:
                shader.setUniformValue(location, cache.get<GLfloat>(i));
                break;
            case Type::Vector2D:
                shader.setUniformValue(location, cache.get<QVector2D>(i));
                break;
            case Type::Vector3D:
                shader.setUniformValue(location, cache.get<QVector3D>(i));
                break;
            case Type::Vector4D:
                shader.setUniformValue(location, cache.get<QVector4D>(i));
                break;
            case Type::Matrix4x4:
                shader.setUniformValue(location, cache.get<QMatrix4x4>(i));
                break;
            }
        }
    }

    for (int i = 0; i < geometry.attributeCount(); i++) {
        int location = material.attributeLocation(i);
        shader.enableAttributeArray(location);
        shader.setAttributeArray(location, geometry.vertexData(i), geometry.tupleSize(i));
    }

    // TODO multiple textures
    auto pTexture = m_pMaterial->texture(1);
    if (pTexture) {
        pTexture->impl().glTexture()->bind();
    }

    glDrawArrays(toGlDrawingMode(geometry.drawingMode()), 0, geometry.vertexCount());

    if (pTexture) {
        pTexture->impl().glTexture()->release();
    }

    for (int i = 0; i < geometry.attributeCount(); i++) {
        int location = material.attributeLocation(i);
        shader.disableAttributeArray(location);
    }

    shader.release();

    Node::Impl::render();
}
