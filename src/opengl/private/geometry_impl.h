#pragma once

#include "rendergraph/geometry.h"
#include "attributeset_impl.h"

class rendergraph::Geometry::Impl {
  public:
    Impl(const AttributeSet& attributeSet, int vertexCount)
            : m_vertexCount(vertexCount) {
        for (const auto& attribute : attributeSet.impl().attributes()) {
            m_data.emplace_back(std::vector<float>(m_vertexCount * attribute.m_tupleSize));
            m_tupleSizes.push_back(attribute.m_tupleSize);
        }
    }

    int attributeCount() const {
        return m_tupleSizes.size();
    }

    int vertexCount() const {
        return m_vertexCount;
    }

    float const* vertexData(int attributeIndex) const {
        return m_data[attributeIndex].data();
    }

    int tupleSize(int attributeIndex) const {
        return m_tupleSizes[attributeIndex];
    }

    void setAttributeValues(int attributePosition, const float* from, int numTuples) {
        memcpy(m_data[attributePosition].data(), from, numTuples * m_tupleSizes[attributePosition] * sizeof(float));
    }

  private:
    int m_vertexCount;
    std::vector<int> m_tupleSizes;
    std::vector<std::vector<float>> m_data;
};

