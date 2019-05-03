#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include "window.hpp"
#include "shaders/shader.hpp"

namespace pr {

    class Looper {

        Window& window;
        Shader& shader;

        void processInput();

        void render();

        void swap();

    public:

        Looper( Window& window, Shader &shader) : window( window ), shader(shader) {}

        Looper( Window&& window, Shader &shader) : window( window ), shader(shader) {}

        void loop();

    };

}

#endif //CG_LOOPER_HPP
