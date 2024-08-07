find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)

qt_add_executable(gl_example
    window.cpp window.h
    main.cpp
)

set_target_properties(gl_example PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(gl_example PRIVATE
    rendergraph_gl
    rendergraph_examples
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
)

qt_add_resources(gl_example "gl_example"
    PREFIX
        /example
    FILES
        images/test.png
)

install(TARGETS gl_example
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET gl_example
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
