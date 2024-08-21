#pragma once

#include <memory>
#include <QQuickWindow>
#include <QSGNode>

namespace rendergraph
{
    class Context;
    class Node;

    std::unique_ptr<Context> createSgContext(QQuickWindow* window);
    QSGNode* sgNode(Node* pNode);
}

