#pragma once

#include "rendergraph/materialshader.h"
#include <QOpenGLShaderProgram>

class rendergraph::MaterialShader::Impl : private QOpenGLShaderProgram {
  public:
    Impl(MaterialShader* pOwner, const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniformSet, const AttributeSet& attributeSet);
    QOpenGLShaderProgram& glShader() {
        return *this;
    }

    int attributeLocation(int attributeIndex) const {
        return m_attributeLocations[attributeIndex];
    }

    int uniformLocation(int uniformIndex) const {
        return m_uniformLocations[uniformIndex];
    }

  private:
    static QString resource(const char* filename) {
        return QString(":/rendergraph_gl/shaders/") + QString(filename) + QString(".gl");
    }
    std::vector<int> m_attributeLocations;
    std::vector<int> m_uniformLocations;
    MaterialShader* m_pOwner;
};

