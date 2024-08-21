#include "rendergraph/uniformset.h"

using namespace rendergraph;

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

const std::vector<Uniform>& UniformSet::uniforms() const {
    return m_uniforms;
}

