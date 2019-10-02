#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include "graphics/eye_light.hpp"
#include "entities/camera.hpp"
#include <vector>
#include "entities/basic_entities.hpp"
#include "graphics/modeling/model.hpp"
#include "graphics/modeling/chunk.hpp"
#include "window.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/shaders/shader.hpp"
#include "graphics/modeling/texture.hpp"
#include "entities/camera.hpp"
#include "graphics/directional_light.hpp"
#include "time_effect.hpp"
#include "good_random.hpp"
#include "utilities.hpp"

#define COLLISION_TEX_SIZE 32
#define COLLISION_VIEW_SIZE 4.0
#include<unistd.h>

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
        long particleId = 0;
        long effectId = 0;
        bool exploding = false;
        Shader shader;
        Shader shadowShader;
        Shader skyboxShader;
        Shader collisionShader;
        Shader particleShader;
        Shader sunShader;
        std::unordered_map< std::string, Model > models;
        std::unordered_map< std::string, Texture > textures;
        std::unordered_map< std::string, Entity > entities;
        std::unordered_map< std::pair<int, int>, Chunk, pair_hash> chunks;
        std::unordered_map< std::string, Particle > particles;
        std::unordered_map< std::string, TimeEffect > effects;
        EyeLight eyeLight;
        std::vector< DirectionalLight > directionalLights;
        unsigned int depthMapFrameBuffer;
        std::unordered_map< MoveDir, std::vector<float>, std::hash<int>> collisionTextures;
        unsigned int collisionMapFrameBuffer;
        unsigned int collisionTexture;

        void processInput();

        void render();

        void swap();

        void drawCube( Shader &shader, glm::mat4 );

        void updateScene();

        void renderChunks( Shader &shader );

        void renderScene( Shader &shader, bool playerUfoVisible=true);

        void renderParticles();

        void renderSkybox();

        void initListeners();

        void initScene();

        std::vector<float> getDepthVector(glm::vec3 dir, bool ufoVisible);

        bool detectCollision(glm::vec3 &translation);

        void initCollisions();

        float maxv=0.0;

        void smokeParticle();

        void electricalParticle();

        void sparksParticle();

        void boomParticle();


        void collisionEffect();

        void smokeEffect();

        void touchEffect();

        void sparkingEffect();

        void boomEffect();

    public:

        Camera* getUFOCamera();

        Looper( Window &window );

        virtual ~Looper();

        void loop();
    };

}

#endif //CG_LOOPER_HPP
