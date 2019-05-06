#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include "entities/camera.hpp"
#include <vector>
#include "entities/basic_entities.hpp"
#include "graphics/modeling/model.hpp"
#include "window.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/modeling/texture.hpp"
#include "entities/camera.hpp"
#include "graphics/directional_light.hpp"

namespace pr {

    class Looper {

        static constexpr double fpsRefresh = 2.;
        Window &window;
        Camera mainCamera;
        int frameCount = 0;
        double framesTime = 0;
        double recentTime = 0;
        double updateTime;
        Shader shader;
        Shader shadowShader;
        std::unordered_map< std::string, Model > models;
        std::vector< Entity > entities;
        std::unordered_map< std::string, Texture > textures;
        std::vector< DirectionalLight > directionalLights;
        unsigned int depthMapFrameBuffer;

        void processInput();

        void render();

        void swap();

        void drawCube( Shader &shader, glm::mat4 );

        void renderScene( Shader &shader );

        void initListeners();

        void initScene();

    public:

        Looper( Window &window );

        virtual ~Looper();

        void loop();
    };

}

#endif //CG_LOOPER_HPP
