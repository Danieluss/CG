#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <iostream>
#include <models/camera.hpp>
#include <vector>
#include "window.hpp"
#include "shaders/shader.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "directionalLight.hpp"

namespace pr {

    class Looper {

        Window& window;
        Shader& shader;
        Camera camera;
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

    public:

        Looper( Window& window);

        ~Looper();

        void loop();

        void init();

    };

}

#endif //CG_LOOPER_HPP
