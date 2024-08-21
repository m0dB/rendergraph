#pragma once

#include "rendergraph/attributeset.h"
#include "rendergraph/uniformset.h"

namespace rendergraph {
class MaterialShader;
}

class rendergraph::MaterialShader {
  public:
    class Impl;

    MaterialShader(const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniforms, const AttributeSet& attributeSet);
    ~MaterialShader();
    Impl& impl() const;

  private:
    MaterialShader(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

