#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "rendergraph.h"

inline int sizeOf(const rendergraph::PrimitiveType& t) {
    switch (t) {
    case rendergraph::PrimitiveType::UInt:
        return sizeof(uint32_t);
    case rendergraph::PrimitiveType::Float:
        return sizeof(float);
    }
    throw "NOT IMPLEMENTED";
}

class rendergraph::AttributeSet::Impl {
  public:
    void add(const Attribute& attribute) {
        m_attributes.push_back(attribute);
    }

    const std::vector<Attribute>& attributes() const {
        return m_attributes;
    }

  private:
    std::vector<Attribute> m_attributes;
};

class rendergraph::MaterialType::Impl {
  public:
    Impl() {
    }
};

class rendergraph::MaterialShader::Impl : private QOpenGLShaderProgram {
  public:
    Impl(MaterialShader* pOwner, const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniformSet, const AttributeSet& attributeSet);
    QOpenGLShaderProgram& glShader() {
        return *this;
    }

    int attributeLocation(int attributeIndex) const {
        return m_attributeLocations[attributeIndex];
    }

    int uniformLocation(int uniformIndex) const {
        return m_uniformLocations[uniformIndex];
    }

  private:
    static QString resource(const char* filename) {
        return QString(":/rendergraph_gl/shaders/") + QString(filename) + QString(".gl");
    }
    std::vector<int> m_attributeLocations;
    std::vector<int> m_uniformLocations;
};

class rendergraph::Material::Impl {
  public:
    Impl(Material* pOwner) {
    }

    void setShader(MaterialShader* pShader) {
        m_pShader = pShader;
    }

    int attributeLocation(int attributeIndex) const {
        return m_pShader->impl().attributeLocation(attributeIndex);
    }

    int uniformLocation(int uniformIndex) const {
        return m_pShader->impl().uniformLocation(uniformIndex);
    }

    QOpenGLShaderProgram& glShader() const {
        return m_pShader->impl().glShader();
    }

  private:
    MaterialShader* m_pShader{};
};

class rendergraph::Geometry::Impl {
  public:
    Impl(const AttributeSet& attributeSet, int vertexCount)
            : m_vertexCount(vertexCount) {
        for (const auto& attribute : attributeSet.impl().attributes()) {
            m_data.emplace_back(std::vector<float>(m_vertexCount * attribute.m_tupleSize));
            m_tupleSizes.push_back(attribute.m_tupleSize);
        }
    }

    int attributeCount() const {
        return m_tupleSizes.size();
    }

    int vertexCount() const {
        return m_vertexCount;
    }

    float const* vertexData(int attributeIndex) const {
        return m_data[attributeIndex].data();
    }

    int tupleSize(int attributeIndex) const {
        return m_tupleSizes[attributeIndex];
    }

    void setAttributeValues(int attributePosition, const float* from, int numTuples) {
        memcpy(m_data[attributePosition].data(), from, numTuples * m_tupleSizes[attributePosition] * sizeof(float));
    }

  private:
    int m_vertexCount;
    std::vector<int> m_tupleSizes;
    std::vector<std::vector<float>> m_data;
};

class rendergraph::Node::Impl {
  public:
    Impl() {
    }

    void appendChildNode(std::unique_ptr<Node> pChild) {
        m_pChildren.emplace_back(std::move(pChild));
    }

    void removeAllChildNodes() {
        m_pChildren.clear();
    }

    virtual void initialize() {
        for (auto& pChild : m_pChildren) {
            pChild->impl().initialize();
        }
    }

    virtual void render() {
        for (auto& pChild : m_pChildren) {
            pChild->impl().render();
        }
    }

  private:
    std::vector<std::unique_ptr<Node>> m_pChildren;
};

class rendergraph::GeometryNode::Impl : public rendergraph::Node::Impl, public QOpenGLFunctions {
  public:
    Impl() {
    }

    void setGeometry(Geometry* geometry) {
        m_pGeometry = geometry;
    }

    void setMaterial(Material* material) {
        m_pMaterial = material;
    }

    void initialize() override {
        initializeOpenGLFunctions();
        m_pMaterial->impl().setShader(m_pMaterial->createShader());
        Node::Impl::initialize();
    }

    void render() override;

  private:
    Geometry* m_pGeometry{};
    Material* m_pMaterial{};
};

class rendergraph::RenderGraph::Impl {
  public:
    Impl(std::unique_ptr<Node> node)
            : m_pTopNode(std::move(node)) {
    }

    void initialize() {
        m_pTopNode->impl().initialize();
    }

    void render() {
        m_pTopNode->impl().render();
    }

  private:
    std::unique_ptr<Node> m_pTopNode;
};
