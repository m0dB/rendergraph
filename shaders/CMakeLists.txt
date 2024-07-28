qt6_add_shaders(rendergraph_sg "shaders-sg"
    BATCHABLE
    PRECOMPILE
    OPTIMIZED
    PREFIX
        "/rendergraph_sg/shaders"
    FILES
        "endoftrack.vert"
        "endoftrack.frag"
)

qt_add_resources(rendergraph_gl "shaders-gl"
    PREFIX
        "/rendergraph_gl/shaders"
    FILES
        "endoftrack.vert.gl"
        "endoftrack.frag.gl"
)
