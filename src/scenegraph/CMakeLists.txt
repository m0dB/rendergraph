add_library(rendergraph_sg
    private/rendergraph_impl.h
    rendergraph_impl.cpp
    ../rendergraph/rendergraph.cpp
    ../rendergraph/rendergraph.h
)

target_link_libraries(rendergraph_sg PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

target_include_directories(rendergraph_sg PUBLIC ../rendergraph PRIVATE private)
