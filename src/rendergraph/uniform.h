#pragma once

#include <QString>

#include "rendergraph/types.h"

namespace rendergraph {
struct Uniform;
}

struct rendergraph::Uniform {
    Type m_type;
    QString m_name;

    Uniform(Type type)
            : m_type(type) {
    }

    template<typename T>
    static Uniform create() {
        return Uniform(typeOf<T>());
    }
};
