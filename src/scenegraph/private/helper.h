#pragma once

#include <QSGGeometry>

inline int toQSGGeometryType(const rendergraph::PrimitiveType& t) {
    switch (t) {
    case rendergraph::PrimitiveType::Float:
        return QSGGeometry::FloatType;
    default:
        break;
    }
    throw "NOT IMPLEMENTED";
}

