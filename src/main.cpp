#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include "window.hpp"
#include "looper.hpp"
#include "event_handling/input_triggers.hpp"
#include "event_handling/listener_manager.hpp"
#include "shaders/shader.hpp"

void _deb( std::ostream &out, std::string &line_separator, std::string &separator ) {
    out<<line_separator;
}

template< class Arg, class ...Args >
void _deb( std::ostream &out,
           std::string line_separator,
           std::string separator,
           Arg &&arg,
           Args &&...args ) {
    out<<separator<<std::forward< Arg >( arg );
    _deb( out, line_separator, separator, std::forward< Args >( args )... );
}

template< class Arg, class ...Args >
void _deb_first( std::ostream &out,
                 std::string line_separator,
                 std::string separator,
                 Arg &&arg,
                 Args &&...args ) {
    out<<std::forward< Arg >( arg );
    _deb( out, line_separator, separator, std::forward< Args >( args )... );
}

template< class ...Args >
void deb( Args &&...args ) {
    _deb_first( std::cout, "\n", " ", std::forward< Args >( args )... );
}

using namespace pr;

int main() {
    try {
        Window::init();
        Window window;
        GLenum err;
        Assimp::Importer importer;
        ListenerManager.hook( window );
        if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
            fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        Shader test;
        Looper looper(window, test);
        while(!glfwWindowShouldClose(window)) {
            looper.loop();
        }
    } catch( std::exception& ex ) {
        deb( ex.what() );
    }
    glfwTerminate();

    return 0;
}