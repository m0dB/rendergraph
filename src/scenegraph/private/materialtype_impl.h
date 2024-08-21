#pragma once

#include "rendergraph/materialtype.h"
#include <QSGMaterial>

class rendergraph::MaterialType::Impl : public QSGMaterialType {
  public:
    QSGMaterialType* sgMaterialType() {
        return this;
    }
};

