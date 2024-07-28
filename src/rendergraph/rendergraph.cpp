#include "rendergraph.h"

#include <QVector2D>
#include <QVector4D>

#include "rendergraph_impl.h"

using namespace rendergraph;

int rendergraph::sizeOf(PrimitiveType type) {
    switch (type) {
    case PrimitiveType::UInt:
        return sizeof(GLuint);
    case PrimitiveType::Float:
        return sizeof(GLfloat);
    }
    return 0;
}

int rendergraph::sizeOf(Type type) {
    switch (type) {
    case Type::UInt:
        return sizeof(GLuint);
    case Type::Float:
        return sizeof(GLfloat);
    case Type::Vector2D:
        return sizeof(QVector2D);
    case Type::Vector3D:
        return sizeof(QVector3D);
    case Type::Vector4D:
        return sizeof(QVector4D);
    case Type::Matrix4x4:
        return sizeof(QMatrix4x4);
    }
    return 0;
}

template<>
Type typeOf<GLuint>() {
    return Type::UInt;
}

template<>
Type typeOf<GLfloat>() {
    return Type::Float;
}

template<>
Type typeOf<QVector2D>() {
    return Type::Vector2D;
}

template<>
Type typeOf<QVector3D>() {
    return Type::Vector3D;
}

template<>
Type typeOf<QVector4D>() {
    return Type::Vector4D;
}

template<>
Type typeOf<QMatrix4x4>() {
    return Type::Matrix4x4;
}

template<>
PrimitiveType primitiveTypeOf<GLfloat>() {
    return PrimitiveType::Float;
}

template<>
PrimitiveType primitiveTypeOf<GLuint>() {
    return PrimitiveType::UInt;
}

template<>
PrimitiveType primitiveTypeOf<QVector2D>() {
    return PrimitiveType::Float;
}
template<>
PrimitiveType primitiveTypeOf<QVector3D>() {
    return PrimitiveType::Float;
}
template<>
PrimitiveType primitiveTypeOf<QVector4D>() {
    return PrimitiveType::Float;
}

template<>
int tupleSizeOf<GLuint>() {
    return 1;
}
template<>
int tupleSizeOf<GLfloat>() {
    return 1;
}
template<>
int tupleSizeOf<QVector2D>() {
    return 2;
}
template<>
int tupleSizeOf<QVector3D>() {
    return 3;
}
template<>
int tupleSizeOf<QVector4D>() {
    return 4;
}

AttributeSet::AttributeSet(AttributeSet::Impl* pImpl)
        : m_pImpl(pImpl) {
}

AttributeSet::AttributeSet()
        : AttributeSet(new AttributeSet::Impl()) {
}

AttributeSet::AttributeSet(std::initializer_list<Attribute> list, const std::vector<QString>& names)
        : AttributeSet() {
    int i = 0;
    for (auto item : list) {
        item.m_name = names[i++];
        add(item);
    }
}

AttributeSet::~AttributeSet() = default;

AttributeSet::Impl& AttributeSet::impl() const {
    return *m_pImpl;
}

void AttributeSet::add(const Attribute& attribute) {
    m_attributes.push_back(attribute);
    m_pImpl->add(attribute);
}

const std::vector<Attribute> AttributeSet::attributes() const {
    return m_attributes;
}

UniformSet::UniformSet(std::initializer_list<Uniform> list, const std::vector<QString>& names) {
    int i = 0;
    for (auto item : list) {
        item.m_name = names[i++];
        add(item);
    }
}

UniformSet::~UniformSet() = default;

void UniformSet::add(const Uniform& uniform) {
    m_uniforms.push_back(uniform);
}

const std::vector<Uniform> UniformSet::uniforms() const {
    return m_uniforms;
}

UniformsCache::UniformsCache(const UniformSet& uniformSet) {
    int offset = 0;
    for (const auto& uniform : uniformSet.uniforms()) {
        const int size = sizeOf(uniform.m_type);
        m_infos.push_back({uniform.m_type, offset});
        offset += size;
    }
    m_byteArray.resize(offset, 0);
}

UniformsCache::~UniformsCache() = default;

void UniformsCache::set(int uniformIndex, Type type, const void* ptr, int size) {
    assert(type == m_infos[uniformIndex].m_type);
    memcpy(m_byteArray.data() + m_infos[uniformIndex].m_offset, ptr, size);
}

void UniformsCache::get(int uniformIndex, Type type, void* ptr, int size) const {
    assert(type == m_infos[uniformIndex].m_type);
    memcpy(ptr, m_byteArray.data() + m_infos[uniformIndex].m_offset, size);
}

MaterialType::MaterialType(Impl* pImpl)
        : m_pImpl(pImpl) {
}

MaterialType::MaterialType()
        : MaterialType(new MaterialType::Impl()){};

MaterialType::~MaterialType() = default;

MaterialType::Impl& MaterialType::impl() const {
    return *m_pImpl;
}

MaterialShader::MaterialShader(Impl* pImpl)
        : m_pImpl(pImpl) {
}

MaterialShader::MaterialShader(const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniformSet, const AttributeSet& attributeSet)
        : MaterialShader(new MaterialShader::Impl(this, vertexShaderFile, fragmentShaderFile, uniformSet, attributeSet)){};

MaterialShader::~MaterialShader() = default;

MaterialShader::Impl& MaterialShader::impl() const {
    return *m_pImpl;
}

Material::Material(Impl* pImpl, const UniformSet& uniformSet)
        : m_pImpl(pImpl), m_uniformsCache(uniformSet) {
}

Material::Material(const UniformSet& uniformSet)
        : Material(new Material::Impl(this), uniformSet) {
}

Material::~Material() = default;

Material::Impl& Material::impl() const {
    return *m_pImpl;
}

Geometry::Geometry(Impl* pImpl)
        : m_pImpl(pImpl) {
}

Geometry::Geometry(const AttributeSet& attributeSet, int vertexCount)
        : Geometry(new Geometry::Impl(attributeSet, vertexCount)){};

Geometry::~Geometry() = default;

Geometry::Impl& Geometry::impl() const {
    return *m_pImpl;
}

void Geometry::setAttributeValues(int attributePosition, const float* data, int numTuples) {
    m_pImpl->setAttributeValues(attributePosition, data, numTuples);
}

Node::Node(Node::Impl* pImpl)
        : m_pImpl(pImpl) {
}

Node::Node()
        : Node(new Node::Impl) {
}

Node::~Node() = default;

Node::Impl& Node::impl() const {
    return *m_pImpl;
}

void Node::appendChildNode(std::unique_ptr<Node> pChild) {
    impl().appendChildNode(std::move(pChild));
}

void Node::removeAllChildNodes() {
    impl().removeAllChildNodes();
}

GeometryNode::GeometryNode(GeometryNode::Impl* pImpl)
        : Node(pImpl) {
}

GeometryNode::GeometryNode()
        : GeometryNode(new GeometryNode::Impl) {
}

GeometryNode::~GeometryNode() = default;

GeometryNode::Impl& GeometryNode::impl() const {
    return static_cast<GeometryNode::Impl&>(Node::impl());
}

void GeometryNode::setGeometry(std::unique_ptr<Geometry> geometry) {
    m_geometry = std::move(geometry);
    impl().setGeometry(m_geometry.get());
}

void GeometryNode::setMaterial(std::unique_ptr<Material> material) {
    m_material = std::move(material);
    impl().setMaterial(m_material.get());
}

Geometry& GeometryNode::geometry() const {
    return *m_geometry;
}

Material& GeometryNode::material() const {
    return *m_material;
}

RenderGraph::RenderGraph(std::unique_ptr<Node> node)
        : m_pImpl(new RenderGraph::Impl(std::move(node))) {
}

RenderGraph::~RenderGraph() = default;

RenderGraph::Impl& RenderGraph::impl() const {
    return *m_pImpl;
}

void RenderGraph::initialize() {
    m_pImpl->initialize();
}

void RenderGraph::render() {
    m_pImpl->render();
}
