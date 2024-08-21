#pragma once

#include "rendergraph/graph.h"
#include "rendergraph/node.h"

#include "node_impl.h"

class rendergraph::Graph::Impl {
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
