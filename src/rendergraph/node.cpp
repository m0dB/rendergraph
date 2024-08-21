#include "rendergraph/node.h"

#include "node_impl.h"

using namespace rendergraph;

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

Node* Node::lastChild() const {
    return impl().lastChild();
}
