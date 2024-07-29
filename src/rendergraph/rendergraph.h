#pragma once

#include <QString>
#include <QImage>
#include <initializer_list>
#include <memory>

namespace rendergraph {
struct Attribute;
class AttributeSet;
struct Uniform;
class UniformSet;
class UniformsCache;
class MaterialType;
class MaterialShader;
class Texture;
class Material;
class Geometry;
class Node;
class GeometryNode;
class RenderGraph;
class Context;

enum class PrimitiveType {
    UInt,
    Float,
};

enum class Type {
    UInt,
    Float,
    Vector2D,
    Vector3D,
    Vector4D,
    Matrix4x4
};

int sizeOf(Type type);
int sizeOf(PrimitiveType primitiveType);

} // namespace rendergraph

template<typename T>
rendergraph::Type typeOf();

template<typename T>
rendergraph::PrimitiveType primitiveTypeOf();

template<typename T>
int tupleSizeOf();

struct rendergraph::Attribute {
    int m_tupleSize;
    PrimitiveType m_primitiveType;
    QString m_name;

    Attribute(int tupleSize, PrimitiveType primitiveType)
            : m_tupleSize(tupleSize), m_primitiveType(primitiveType) {
    }

    template<typename T>
    static Attribute create() {
        return Attribute(tupleSizeOf<T>(), primitiveTypeOf<T>());
    }
};

class rendergraph::AttributeSet {
  public:
    class Impl;

    AttributeSet();
    AttributeSet(std::initializer_list<Attribute> list, const std::vector<QString>& names);

    ~AttributeSet();

    const std::vector<Attribute>& attributes() const;
    Impl& impl() const;

  private:
    AttributeSet(Impl* pImpl);
    void add(const Attribute& attribute);

    std::vector<Attribute> m_attributes;
    const std::unique_ptr<Impl> m_pImpl;
};

namespace rendergraph {
template<typename... T>
AttributeSet makeAttributeSet(const std::vector<QString>& names) {
    return AttributeSet({(Attribute::create<T>())...}, names);
}
}

struct rendergraph::Uniform {
    Type m_type;
    QString m_name;

    Uniform(Type type)
            : m_type(type) {
    }

    template<typename T>
    static Uniform create() {
        return Uniform(typeOf<T>());
    }
};

class rendergraph::UniformSet {
  public:
    UniformSet(std::initializer_list<Uniform> list, const std::vector<QString>& names);

    ~UniformSet();

    const std::vector<Uniform>& uniforms() const;

  private:
    void add(const Uniform& uniform);
    std::vector<Uniform> m_uniforms;
};

namespace rendergraph {
template<typename... T>
UniformSet makeUniformSet(const std::vector<QString>& names) {
    return UniformSet({(Uniform::create<T>())...}, names);
}
}

class rendergraph::UniformsCache {
  public:
    UniformsCache(const UniformSet& uniformSet);
    ~UniformsCache();

    template<typename T>
    void set(int uniformIndex, const T& value) {
        set(uniformIndex, typeOf<T>(), static_cast<const void*>(&value), sizeof(T));
    }

    template<typename T>
    T get(int uniformIndex) const {
        T value;
        get(uniformIndex, typeOf<T>(), static_cast<void*>(&value), sizeof(T));
        return value;
    }
    Type type(int uniformIndex) const {
        return m_infos[uniformIndex].m_type;
    }

    const char* data() const {
        return m_byteArray.data();
    }
    qsizetype size() const {
        return m_byteArray.size();
    }
    int count() const {
        return m_infos.size();
    }

  private:
    void set(int uniformIndex, Type type, const void* ptr, int size);
    void get(int uniformIndex, Type type, void* ptr, int size) const;

    struct Info {
        Type m_type;
        int m_offset;
    };

    std::vector<Info> m_infos;
    QByteArray m_byteArray;
};

class rendergraph::MaterialType {
  public:
    class Impl;

    MaterialType();
    ~MaterialType();
    Impl& impl() const;

  private:
    MaterialType(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

class rendergraph::MaterialShader {
  public:
    class Impl;

    MaterialShader(const char* vertexShaderFile, const char* fragmentShaderFile, const UniformSet& uniforms, const AttributeSet& attributeSet);
    ~MaterialShader();
    Impl& impl() const;

  private:
    MaterialShader(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

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

class rendergraph::Material {
  public:
    class Impl;

    Material(const UniformSet& uniformSet);
    virtual ~Material();
    virtual int compare(const Material* other) const = 0;
    virtual MaterialShader* createShader() const = 0;
    virtual MaterialType* type() const = 0;

    template<typename T>
    void setUniform(int uniformIndex, const T& value) {
        m_uniformsCache.set(uniformIndex, value);
        m_uniformsCacheDirty = true;
    }

    Impl& impl() const;
    const UniformsCache& uniformsCache() const {
        return m_uniformsCache;
    }

    bool clearUniformsCacheDirty() {
        if (m_uniformsCacheDirty) {
            m_uniformsCacheDirty = false;
            return true;
        }
        return false;
    }

    virtual Texture* texture(int binding) const {
        return nullptr;
    }

  private:
    Material(Impl* pImpl, const UniformSet& uniformSet);

    const std::unique_ptr<Impl> m_pImpl;
    UniformsCache m_uniformsCache;
    bool m_uniformsCacheDirty{};
};

class rendergraph::Geometry {
  public:
    class Impl;

    Geometry(const AttributeSet& attributeSet, int vertexCount);
    ~Geometry();
    void setAttributeValues(int attributePosition, const float* data, int numTuples);
    Impl& impl() const;

  private:
    Geometry(Impl* pImpl);

    const std::unique_ptr<Impl> m_pImpl;
};

class rendergraph::Node {
  public:
    class Impl;

    Node();
    ~Node();
    Impl& impl() const;
    void appendChildNode(std::unique_ptr<Node> pChild);
    void removeAllChildNodes();
    Node* lastChild() const;
  protected:
    Node(Impl* impl);

  private:
    const std::unique_ptr<Impl> m_pImpl;
};

class rendergraph::GeometryNode : public rendergraph::Node {
  public:
    class Impl;

    GeometryNode();
    ~GeometryNode();
    Impl& impl() const;
    void setMaterial(std::unique_ptr<Material> material);
    void setGeometry(std::unique_ptr<Geometry> geometry);
    Geometry& geometry() const;
    Material& material() const;

  private:
    GeometryNode(Impl* pImpl);

    std::unique_ptr<Material> m_material;
    std::unique_ptr<Geometry> m_geometry;
};

class rendergraph::RenderGraph {
  public:
    class Impl;
    RenderGraph(std::unique_ptr<Node> node);
    ~RenderGraph();
    Impl& impl() const;
    void initialize();
    void render();

  private:
    const std::unique_ptr<Impl> m_pImpl;
};

class rendergraph::Context {
  public:
    class Impl;
    Context();
    ~Context();
    Impl& impl() const;
  private:
    const std::unique_ptr<Impl> m_pImpl;
};

