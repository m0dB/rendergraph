#pragma once

#include "rendergraph/node.h"
#include <vector>

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

    Node* lastChild() const {
        return m_pChildren.back().get();
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

