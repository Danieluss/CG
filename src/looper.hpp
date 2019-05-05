#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include "objects/camera.hpp"
#include <vector>
#include "window.hpp"
#include "shaders/shader.hpp"
#include "shaders/shader.hpp"
#include "objects/modeling/texture.hpp"
#include "objects/directional_light.hpp"

namespace pr {

    class Looper {

        Window& window;
        Camera mainCamera;
        double recentTime = 0;
        double updateTime;
        Shader shader;
        Shader shadowShader;
        std::vector<Texture> textures;
        std::vector<DirectionalLight> directionalLights;
        unsigned int depthMapFrameBuffer;

        void processInput();

        void render();

        void swap();

        void drawCube(Shader &shader, glm::mat4);

        void renderScene(Shader &shader);

        void initListeners();

        void initScene();

    public:

        Looper( Window& window);

        virtual ~Looper();

        void loop();
    };

}

#endif //CG_LOOPER_HPP
