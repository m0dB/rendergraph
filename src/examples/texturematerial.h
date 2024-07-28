#include "rendergraph.h"

namespace rendergraph {
class TextureMaterial;
}

class rendergraph::TextureMaterial : public rendergraph::Material {
  public:
    TextureMaterial();

    static const AttributeSet& attributes();

    static const UniformSet& uniforms();

    MaterialType* type() const override;

    int compare(const Material* other) const override;

    MaterialShader* createShader() const override;

    Texture* getTexture(int binding) const override
    {
        return m_pTexture.get();
    }

    void setTexture(std::unique_ptr<Texture> texture)
    {
        m_pTexture = std::move(texture);
    }
  private:
    std::unique_ptr<Texture> m_pTexture;
};
