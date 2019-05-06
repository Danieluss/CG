#include "window.hpp"
#include<iostream>

#define deb( x ) std::cout << x << std::endl
namespace pr {

    int Window::__init;
    const char *Window::DEF_NAME = "App";

    void Window::init() {
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        __init = glfwInit();
    }

    Window::Window( unsigned width,
                    unsigned height,
                    const char *name,
                    bool makeContext ) : width( width ), height( height ) {
        gwindow = glfwCreateWindow( width, height, name, NULL, NULL );
        if( makeContext ) {
            glfwMakeContextCurrent( gwindow );
            glfwFocusWindow( gwindow );
        }
        if( __init == -1 || !gwindow )
            throw ( "bad init" );
        glClearColor( 0.2, 0.3, 0.3, 1 );
        glEnable( GL_DEPTH_TEST );
        glfwSetInputMode( gwindow, GLFW_STICKY_KEYS, GLFW_TRUE );
        glfwSetInputMode( gwindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE );
        glfwSetInputMode( gwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
    }

    glm::vec2 Window::size() {
        int width, height;
        glfwGetWindowSize( gwindow, &width, &height );
        return glm::vec2( width, height );
    };
}
