#pragma once

#include <memory>

namespace rendergraph {
class Graph;
class Node;
} // namespace rendergraph

class rendergraph::Graph {
  public:
    class Impl;
    Graph(std::unique_ptr<Node> node);
    ~Graph();
    Impl& impl() const;
    void initialize();
    void render();

  private:
    const std::unique_ptr<Impl> m_pImpl;
};
