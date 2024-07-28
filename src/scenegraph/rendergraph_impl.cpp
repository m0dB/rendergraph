#include "rendergraph_impl.h"

using namespace rendergraph;

QSGNode* native(Node* pNode) {
    return pNode->impl().sgNode();
}
