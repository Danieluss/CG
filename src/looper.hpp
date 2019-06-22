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

#define COLLISION_TEX_SIZE 16

namespace pr {

    class Looper {

        static constexpr double fpsRefresh = 2.;
        Window &window;
        Camera freeCamera;
        UFOCamera ufoCamera;
        ThirdPersonCamera thirdPersonCamera;
        Camera* previousCamera = nullptr;
        Camera* currentCamera = &ufoCamera;
        int frameCount = 0;
        double deltaTime = 0;
        double framesTime = 0;
        double recentTime = 0;
        double updateTime = 0;
        Shader shader;
        Shader shadowShader;
        Shader skyboxShader;
        Shader collisionShader;
        std::unordered_map< std::string, Model > models;
        std::unordered_map< std::string, Texture > textures;
        std::unordered_map< std::string, Entity > entities;
        std::vector< DirectionalLight > directionalLights;
        unsigned int depthMapFrameBuffer;
       // std::unordered_map< MoveDir, unsigned int> collisionTextures;

        void processInput();

        void render();

        void swap();

        void drawCube( Shader &shader, glm::mat4 );

        void updateScene();

        void renderScene( Shader &shader );

        void renderSkybox();

        void initListeners();

        void initScene();

        bool detectCollision(MoveDir dir);

        void initCollisions();

    public:

        Looper( Window &window );

        virtual ~Looper();

        void loop();
    };

}

#endif //CG_LOOPER_HPP
