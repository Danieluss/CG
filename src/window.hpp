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

        static void init();

        Window( int width = DEF_WIDTH,
                int height = DEF_HEIGHT,
                const char *name = DEF_NAME,
                bool makeContext = true );

        Window( const char* name, bool makeContext ) : Window( DEF_WIDTH, DEF_HEIGHT, name, makeContext ){}

        Window( const char* name ) : Window( DEF_WIDTH, DEF_HEIGHT, name, DEF_CONTEXT ){}

        Window( bool makeContext ) : Window( DEF_WIDTH, DEF_HEIGHT, DEF_NAME, makeContext ){}

        operator GLFWwindow*() const {
            return gwindow;
        }

        virtual ~Window() {
            glfwDestroyWindow( gwindow );
        }

        glm::vec2 size();

    };

}

#endif //CG_WINDOW_HPP
