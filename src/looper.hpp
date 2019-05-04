#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include <models/camera.hpp>
#include "window.hpp"
#include "shaders/shader.hpp"

namespace pr {

    class Looper {

        Window& window;
        Shader& shader;
        Camera camera;
        double recentTime = 0;
        double updateTime;

        void processInput();

        void render();

        void swap();

    public:

        Looper( Window& window, Shader &shader ) : window( window ), shader( shader ) {
            init();
        }

        Looper( Window&& window, Shader &shader ) : window( window ), shader( shader ) {
            init();
        }

        void loop();

        void init();

    };

}

#endif //CG_LOOPER_HPP
