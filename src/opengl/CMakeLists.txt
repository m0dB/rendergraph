add_library(rendergraph_gl
    rendergraph_impl.cpp
    private/rendergraph_impl.h
    ../rendergraph/rendergraph.cpp
    ../rendergraph/rendergraph.h
)

target_link_libraries(rendergraph_gl PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
)

target_include_directories(rendergraph_gl PUBLIC ../rendergraph PRIVATE private)
