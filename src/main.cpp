#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "window.hpp"
#include "looper.hpp"
#include "input_triggers.hpp"
#include "listener_manager.hpp"
#include "shader.hpp"

using namespace pr;

#define deb(x) std::cout << x << std::endl

int main() {
    try {
        Window::init();
        Window window;
        GLenum err;
        if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
            fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        Shader test;
        Looper looper(window, test);
        ListenerManager::instance().addButtonObs(
                ButtonObserver()
                    .callback( [](int key, int action, int mods)->void{ std::cout<<action<<std::endl;} )
                    .trigger( build< ButtonTrigger >()
                            .key( GLFW_KEY_UP )
                            .action( GLFW_PRESS )
                            .get() ) );
        ListenerManager::instance().hook( window );
        while(!glfwWindowShouldClose(window)) {
            looper.loop();
        }
    } catch( ... ) {

    }
    glfwTerminate();

    return 0;
}