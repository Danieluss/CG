#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include "window.hpp"

namespace pr {

    class Looper {

        Window& window;

        void processInput() {
            glfwPollEvents();
        }

        void render() {
            glClearColor( 0.2, 0.3, 0.3, 1.0 );
            glClear( GL_COLOR_BUFFER_BIT );
        }

        void swap() {
            glfwSwapBuffers( window );
        }

    public:

        Looper( Window& window ) : window( window ) {}

        Looper( Window&& window ) : window( window ) {}

        void loop() {
            processInput();
            render();
            swap();
        }

    };

}

#endif //CG_LOOPER_HPP
