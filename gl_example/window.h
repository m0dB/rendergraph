#pragma once

#include <QOpenGLWindow>
#include <memory>

namespace rendergraph {
class RenderGraph;
}

class Window : public QOpenGLWindow {
  public:
    Window();
    ~Window();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    std::unique_ptr<rendergraph::RenderGraph> m_rendergraph;
};
