#pragma once

#include "rendergraph/geometry.h"
#include "attributeset_impl.h"
#include <QSGGeometry>

class rendergraph::Geometry::Impl : public QSGGeometry {
  public:
    Impl(const rendergraph::AttributeSet& rgAttributeSet, int vertexCount)
            : QSGGeometry(rgAttributeSet.impl().sgAttributeSet(), vertexCount), m_stride(rgAttributeSet.impl().sgAttributeSet().stride) {
        QSGGeometry::setDrawingMode(QSGGeometry::DrawTriangleStrip);
    }

    QSGGeometry* sgGeometry() {
        return this;
    }

    void setAttributeValues(int attributePosition, const float* from, int numTuples) {
        const auto attributeArray = QSGGeometry::attributes();
        int offset = 0;
        for (int i = 0; i < attributePosition; i++) {
            offset += attributeArray[i].tupleSize;
        }
        const int tupleSize = attributeArray[attributePosition].tupleSize;
        const int skip = m_stride / sizeof(float) - tupleSize;

        float* to = static_cast<float*>(QSGGeometry::vertexData());
        to += offset;
        while (numTuples--) {
            int k = tupleSize;
            while (k--) {
                *to++ = *from++;
            }
            to += skip;
        }
    }

  private:
    const int m_stride;
};

