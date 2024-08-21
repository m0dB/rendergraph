#pragma once

#include <QString>
#include "rendergraph/types.h"

namespace rendergraph {
struct Attribute;
}

struct rendergraph::Attribute {
    int m_tupleSize;
    PrimitiveType m_primitiveType;
    QString m_name;

    Attribute(int tupleSize, PrimitiveType primitiveType)
            : m_tupleSize(tupleSize), m_primitiveType(primitiveType) {
    }

    template<typename T>
    static Attribute create() {
        return Attribute(tupleSizeOf<T>(), primitiveTypeOf<T>());
    }
};

