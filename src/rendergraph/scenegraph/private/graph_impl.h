#pragma once

#include "rendergraph/graph.h"
#include "rendergraph/node.h"

class rendergraph::Graph::Impl {
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
