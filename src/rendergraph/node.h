#pragma once

#include <memory>

namespace rendergraph {
class Node;
}

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
