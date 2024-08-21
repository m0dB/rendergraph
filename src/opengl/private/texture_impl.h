#pragma once

#include "rendergraph/texture.h"
#include <QOpenGLTexture>

class rendergraph::Texture::Impl {
public:
    Impl(Context& context, const QImage& image)
    : m_pTexture(new QOpenGLTexture(image.convertToFormat(QImage::Format_ARGB32_Premultiplied)))
    {
    }

    QOpenGLTexture* glTexture() const {
        return m_pTexture.get();
    }
private:
    std::unique_ptr<QOpenGLTexture> m_pTexture{};
};

