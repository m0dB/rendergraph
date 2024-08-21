#pragma once

#include "rendergraph/attributeset.h"

namespace rendergraph {
class Geometry;
}

class rendergraph::Geometry {
  public:
    class Impl;

    Geometry(const AttributeSet& attributeSet, int vertexCount);
    ~Geometry();
    void setAttributeValues(int attributePosition, const float* data, int numTuples);
    Impl& impl() const;

  private:
    Geometry(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

