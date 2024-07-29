#include "rendergraph_impl.h"
#include <QDebug>

namespace rendergraph
{
    // TODO move to a header
    std::unique_ptr<Context> createSgContext(QQuickWindow* window);
    QSGNode* sgNode(Node* pNode);
}

using namespace rendergraph;

QSGNode* rendergraph::sgNode(Node* pNode) {
    return pNode->impl().sgNode();
}

void MaterialShader::Impl::updateSampledImage(RenderState &state, int binding, QSGTexture **texture,
                            QSGMaterial *newMaterial, QSGMaterial *oldMaterial) 
{
    Material::Impl* pMaterialImpl = static_cast<Material::Impl*>(newMaterial);
    *texture = pMaterialImpl->texture(binding);
    (*texture)->commitTextureOperations(state.rhi(), state.resourceUpdateBatch());
}

Texture::Impl::Impl(Context& context, const QImage& image)
: m_pTexture(context.impl().window()->createTextureFromImage(image))
{
}

std::unique_ptr<Context> rendergraph::createSgContext(QQuickWindow* window)
{
    auto context = std::make_unique<Context>();
    context->impl().setWindow(window);
    return context;
}

