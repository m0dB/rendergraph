#include "rendergraph/graph.h"
#include "graph_impl.h"

using namespace rendergraph;

Graph::Graph(std::unique_ptr<Node> node)
        : m_pImpl(new Graph::Impl(std::move(node))) {
}

Graph::~Graph() = default;

Graph::Impl& Graph::impl() const {
    return *m_pImpl;
}

void Graph::initialize() {
    m_pImpl->initialize();
}

void Graph::render() {
    m_pImpl->render();
}

