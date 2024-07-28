#include "endoftrackmaterial.h"

#include <QVector2D>

using namespace rendergraph;

EndOfTrackMaterial::EndOfTrackMaterial()
        : Material(uniforms()) {
}

/* static */ const ::AttributeSet& EndOfTrackMaterial::attributes() {
    static ::AttributeSetWith<QVector2D, float> set({"position", "gradient"});
    return set;
}

/* static */ const ::UniformSet& EndOfTrackMaterial::uniforms() {
    static ::UniformSetWith<QVector4D> set({"ubuf.color"});
    return set;
}

MaterialType* EndOfTrackMaterial::type() const {
    static MaterialType type;
    return &type;
}

int EndOfTrackMaterial::compare(const Material* other) const {
    Q_ASSERT(other && type() == other->type());
    const auto* otherCasted = static_cast<const EndOfTrackMaterial*>(other);
    return otherCasted == this ? 0 : 1;
}

MaterialShader* EndOfTrackMaterial::createShader() const {
    return new MaterialShader("endoftrack.vert", "endoftrack.frag", uniforms(), attributes());
}
