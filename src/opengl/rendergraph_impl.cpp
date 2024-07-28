#include "rendergraph_impl.h"

using namespace rendergraph;

MaterialShader::Impl::Impl(MaterialShader* pOwner, const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniformSet, const AttributeSet& attributeSet) {
    addShaderFromSourceFile(QOpenGLShader::Vertex, resource(vertexShaderFile));
    addShaderFromSourceFile(QOpenGLShader::Fragment, resource(fragmentShaderFile));
    link();
    for (const auto& attribute : attributeSet.attributes()) {
        int location = QOpenGLShaderProgram::attributeLocation(attribute.m_name);
        m_attributeLocations.push_back(location);
    }
    for (const auto& uniform : uniformSet.uniforms()) {
        int location = QOpenGLShaderProgram::uniformLocation(uniform.m_name);
        m_uniformLocations.push_back(location);
    }
}

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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry.vertexCount());

    for (int i = 0; i < geometry.attributeCount(); i++) {
        int location = material.attributeLocation(i);
        shader.disableAttributeArray(location);
    }

    shader.release();

    Node::Impl::render();
}
