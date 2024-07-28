#include "texturematerial.h"

#include <QVector2D>
#include <QMatrix4x4>

using namespace rendergraph;

TextureMaterial::TextureMaterial()
        : Material(uniforms()) {
}

/* static */ const ::AttributeSet& TextureMaterial::attributes() {
    static ::AttributeSetWith<QVector2D, QVector2D> set({"position", "texcoord"});
    return set;
}

/* static */ const ::UniformSet& TextureMaterial::uniforms() {
    static ::UniformSetWith<QMatrix4x4> set({"ubuf.matrix"});
    return set;
}

MaterialType* TextureMaterial::type() const {
    static MaterialType type;
    return &type;
}

int TextureMaterial::compare(const Material* other) const {
    Q_ASSERT(other && type() == other->type());
    const auto* otherCasted = static_cast<const TextureMaterial*>(other);
    return otherCasted == this ? 0 : 1;
}

MaterialShader* TextureMaterial::createShader() const {
    return new MaterialShader("texture.vert", "texture.frag", uniforms(), attributes());
}
