#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGMaterial>
#include <QtQuick/QSGTexture>

#include "rendergraph.h"

inline int toQSGGeometryType(const rendergraph::PrimitiveType& t) {
    switch (t) {
    case rendergraph::PrimitiveType::Float:
        return QSGGeometry::FloatType;
    default:
        break;
    }
    throw "NOT IMPLEMENTED";
}

class rendergraph::AttributeSet::Impl {
  public:
    void add(const Attribute& attribute) {
        const int count = static_cast<int>(m_sgAttributes.size());
        const bool isPosition = count == 0;
        m_sgAttributes.push_back(QSGGeometry::Attribute::create(count, attribute.m_tupleSize, toQSGGeometryType(attribute.m_primitiveType), isPosition));
        const int stride = m_sgAttributeSet.stride + attribute.m_tupleSize * sizeOf(attribute.m_primitiveType);
        m_sgAttributeSet = QSGGeometry::AttributeSet{count + 1, stride, m_sgAttributes.data()};
    }

    const QSGGeometry::AttributeSet& sgAttributeSet() const {
        return m_sgAttributeSet;
    }

  private:
    QSGGeometry::AttributeSet m_sgAttributeSet{};
    std::vector<QSGGeometry::Attribute> m_sgAttributes;
};

class rendergraph::MaterialType::Impl : public QSGMaterialType {
  public:
    QSGMaterialType* sgMaterialType() {
        return this;
    }
};

class rendergraph::MaterialShader::Impl : public QSGMaterialShader {
  public:
    Impl(MaterialShader* pOwner, const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniformSet, const AttributeSet& attributeSet)
            : m_pOwner(pOwner) {
        (void)uniformSet;
        (void)attributeSet;
        setShaderFileName(VertexStage, resource(vertexShaderFile));
        setShaderFileName(FragmentStage, resource(fragmentShaderFile));
    }

    QSGMaterialShader* sgMaterialShader() {
        return this;
    }

  private:
    bool updateUniformData(RenderState& state,
            QSGMaterial* newMaterial,
            QSGMaterial* oldMaterial) override;

    void updateSampledImage(RenderState &state, int binding, QSGTexture **texture,
                            QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

    static QString resource(const char* filename) {
        return QString(":/rendergraph_sg/shaders/") + QString(filename) + QString(".qsb");
    }

    MaterialShader* m_pOwner;
};

// We can't use inheritance because QSGTexture has pure virtuals that we can't implement.
// Encapsulate instead.
class rendergraph::Texture::Impl {
public:
    Impl(Context& context, const QImage& image);

    QSGTexture* sgTexture() const {
        return m_pTexture.get();
    }
private:
    std::unique_ptr<QSGTexture> m_pTexture{};
};

class rendergraph::Material::Impl : public QSGMaterial {
  public:
    Impl(Material* pOwner)
            : m_pOwner(pOwner) {
        setFlag(QSGMaterial::Blending);
    }

    QSGMaterial* sgMaterial() {
        return this;
    }

    bool updateUniformsByteArray(QByteArray* buf) {
        if (m_pOwner->clearUniformsCacheDirty()) {
            memcpy(buf->data(), m_pOwner->uniformsCache().data(), m_pOwner->uniformsCache().size());
            return true;
        }
        return false;
    }
    
    QSGTexture* texture(int binding) {
        return m_pOwner->texture(binding)->impl().sgTexture();
    }
  private:
    QSGMaterialType* type() const override {
        return m_pOwner->type()->impl().sgMaterialType();
    }

    int compare(const QSGMaterial* other) const override {
        const Impl* otherCasted = static_cast<const Impl*>(other);
        return otherCasted && m_pOwner->compare(otherCasted->m_pOwner);
    }

    QSGMaterialShader* createShader(QSGRendererInterface::RenderMode) const override {
        return m_pOwner->createShader()->impl().sgMaterialShader();
    }

    Material* m_pOwner;
};

class rendergraph::Geometry::Impl : public QSGGeometry {
  public:
    Impl(const rendergraph::AttributeSet& rgAttributeSet, int vertexCount)
            : QSGGeometry(rgAttributeSet.impl().sgAttributeSet(), vertexCount), m_stride(rgAttributeSet.impl().sgAttributeSet().stride) {
        QSGGeometry::setDrawingMode(QSGGeometry::DrawTriangleStrip);
    }

    QSGGeometry* sgGeometry() {
        return this;
    }

    void setAttributeValues(int attributePosition, const float* from, int numTuples) {
        const auto attributeArray = QSGGeometry::attributes();
        int offset = 0;
        for (int i = 0; i < attributePosition; i++) {
            offset += attributeArray[i].tupleSize;
        }
        const int tupleSize = attributeArray[attributePosition].tupleSize;
        const int skip = m_stride / sizeof(float) - tupleSize;

        float* to = static_cast<float*>(QSGGeometry::vertexData());
        to += offset;
        while (numTuples--) {
            int k = tupleSize;
            while (k--) {
                *to++ = *from++;
            }
            to += skip;
        }
    }

  private:
    const int m_stride;
};

class rendergraph::Node::Impl {
  public:
    Impl()
            : Impl(new QSGNode()) {
    }
    QSGNode* sgNode() {
        return m_sgNode.get();
    }
    void appendChildNode(std::unique_ptr<Node> pChild) {
        m_sgNode->appendChildNode(pChild->impl().sgNode());
        m_pChildren.emplace_back(std::move(pChild));
    }
    void removeAllChildNodes() {
        m_sgNode->removeAllChildNodes();
        m_pChildren.clear();
    }

    Node* lastChild() const {
        return m_pChildren.back().get();
    }

  protected:
    Impl(QSGNode* node)
            : m_sgNode(node) {
    }

  private:
    std::unique_ptr<QSGNode> m_sgNode;
    std::vector<std::unique_ptr<Node>> m_pChildren;
};

class rendergraph::GeometryNode::Impl : public rendergraph::Node::Impl {
  public:
    Impl()
            : Node::Impl(new QSGGeometryNode()) {
    }
    QSGGeometryNode* sgGeometryNode() {
        return static_cast<QSGGeometryNode*>(sgNode());
    }
    void setGeometry(Geometry* geometry) {
        sgGeometryNode()->setGeometry(geometry->impl().sgGeometry());
    }
    void setMaterial(Material* material) {
        sgGeometryNode()->setMaterial(material->impl().sgMaterial());
    }
};

inline bool rendergraph::MaterialShader::Impl::updateUniformData(RenderState& state,
        QSGMaterial* newMaterial,
        QSGMaterial* oldMaterial) {
    Material::Impl* pMaterialImpl = static_cast<Material::Impl*>(newMaterial);
    return pMaterialImpl->updateUniformsByteArray(state.uniformData());
}

class rendergraph::RenderGraph::Impl {
  public:
    Impl(std::unique_ptr<Node> node)
            : m_pNode(std::move(node)) {
    }
    void initialize() {
    }
    void render() {
    }

  private:
    std::unique_ptr<Node> m_pNode;
};

class rendergraph::Context::Impl {
  public:
    void setWindow(QQuickWindow* pWindow)
    {
        m_pWindow = pWindow;
    }
    QQuickWindow* window() const {
        return m_pWindow;
    }
  private:
    QQuickWindow* m_pWindow;
};

