#pragma once

#include "rendergraph/node.h"
#include <QSGNode>

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

