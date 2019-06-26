#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include "window.hpp"
#include "looper.hpp"
#include "event_handling/input_triggers.hpp"
#include "event_handling/listener_manager.hpp"
#include "graphics/shaders/shader.hpp"

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
        glfwWindowHint(GLFW_SAMPLES, 2);
        Window::init();
        Window window;
        GLenum err;
        ListenerManager.hook( window );
        if(( err = glewInit()) != GLEW_OK ) { //Initialize GLEW library
            fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString( err ));
            exit( EXIT_FAILURE );
        }
        Looper looper( window );
        fflush( stdout );
        while( !glfwWindowShouldClose( window )) {
            looper.loop();
        }
    } catch( const char *ex ) {
        deb( ex );
    } catch( std::string &ex ) {
        deb( ex );
    } catch( std::exception &ex ) {
        deb( ex.what());
    } catch( ... ) {
        deb( "Unexpected exception" );
    }
    std::cout << std::endl;
    glfwTerminate();

    return 0;
}