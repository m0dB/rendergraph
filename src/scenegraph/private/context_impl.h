#pragma once

#include "rendergraph/context.h"
#include <QQuickWindow>

class rendergraph::Context::Impl {
  public:
    void setWindow(QQuickWindow* pWindow)
    {
        m_pWindow = pWindow;
    }
    QQuickWindow* window() const {
        return m_pWindow;
    }
  private:
    QQuickWindow* m_pWindow;
};

