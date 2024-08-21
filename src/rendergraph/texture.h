#pragma once

#include "rendergraph/context.h"
#include <QImage>

namespace rendergraph {
class Texture;
}

class rendergraph::Texture {
  public:
    class Impl;

    Texture(Context& context, const QImage& image);
    ~Texture();
    Impl& impl() const;
  private:
    Texture(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

