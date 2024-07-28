#include "endoftrackmaterial.h"
#include "rendergraph.h"

namespace rendergraph {
class ExampleNode1;
class ExampleNode2;
} // namespace rendergraph

class rendergraph::ExampleNode1 : public rendergraph::GeometryNode {
  public:
    static constexpr float positionArray[] = {-1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f};
    static constexpr float verticalGradientArray[] = {1.f, 1.f, -1.f, -1.f};
    static constexpr float horizontalGradientArray[] = {-1.f, 1.f, -1.f, 1.f};

    ExampleNode1();
};

class rendergraph::ExampleNode2 : public rendergraph::GeometryNode {
  public:
    static constexpr float positionArray[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f};
    static constexpr float verticalGradientArray[] = {1.f, 1.f, 0.f, 0.f};
    static constexpr float horizontalGradientArray[] = {-1.f, 1.f, -1.f, 1.f};

    ExampleNode2();
};
