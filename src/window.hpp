#ifndef CG_WINDOW_HPP
#define CG_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

namespace pr {

    struct Window {

        static const unsigned DEF_WIDTH = 1024;
        static const unsigned DEF_HEIGHT = 576;
        static const char* DEF_NAME;
        static const bool DEF_CONTEXT = true;

        GLFWwindow* gwindow;

        static int __init;

        static void init() {
            __init = glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        Window( int width = DEF_WIDTH,
                int height = DEF_HEIGHT,
                const char *name = DEF_NAME,
                bool makeContext = true ) {
            gwindow = glfwCreateWindow( width, height, name,
                                       NULL, NULL );
            if( makeContext )
                glfwMakeContextCurrent( gwindow );
            if( __init == -1 || !gwindow )
                throw ( "bad init" );
        };

        Window( const char* name, bool makeContext ) : Window( DEF_WIDTH, DEF_HEIGHT, name, makeContext ){}

        Window( const char* name ) : Window( DEF_WIDTH, DEF_HEIGHT, name, DEF_CONTEXT ){}

        Window( bool makeContext ) : Window( DEF_WIDTH, DEF_HEIGHT, DEF_NAME, makeContext ){}

        operator GLFWwindow*() const {
            return gwindow;
        }

        virtual ~Window() {
            glfwDestroyWindow( gwindow );
        }

    };

}

#endif //CG_WINDOW_HPP
