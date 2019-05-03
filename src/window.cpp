#include "window.hpp"
#include<iostream>
#define deb(x) std::cout << x << std::endl
namespace pr {

    int Window::__init;
    const char* Window::DEF_NAME = "App";

    void Window::init() {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            __init = glfwInit();
        }

    Window::Window( int width,
                int height,
                const char *name,
                bool makeContext) {
            gwindow = glfwCreateWindow( width, height, name, NULL, NULL );
            if( makeContext )
                glfwMakeContextCurrent( gwindow );
            if( __init == -1 || !gwindow )
                 throw ( "bad init" );
            glClearColor(0.2,0.3,0.3,1);
	        glEnable(GL_DEPTH_TEST);
    };
}
