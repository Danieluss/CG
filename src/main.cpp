#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "window.hpp"
#include "looper.hpp"
#include "input_triggers.hpp"
#include "listener_manager.hpp"

using namespace pr;

int main() {
    try {
        Window::init();
        Window window;
        Looper looper( window );
        ListenerManager::instance().addButtonObs(
                ButtonObserver()
                    .callback( [](int key, int action, int mods)->void{ std::cout<<action<<std::endl;} )
                    .trigger( build< ButtonTrigger >()
                            .key( GLFW_KEY_UP )
                            .action( GLFW_PRESS )
                            .get() ) );
        ListenerManager::instance().hook( window );
        while( !glfwWindowShouldClose( window )) {
            looper.loop();
        }
    } catch( ... ) {

    }
    glfwTerminate();

    return 0;
}